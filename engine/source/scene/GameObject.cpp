#include "scene/GameObject.h"
#include "Engine.h"
#include "graphics/VertexLayout.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

namespace eng
{
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			component->Update(deltaTime);
		}
		for (auto iterator = children.begin(); iterator != children.end();) //Loops through vector of gameobject children
		{
			if ((*iterator)->GetIsAlive()) //Updates it if its alive
			{
				(*iterator)->Update(deltaTime);
				++iterator;
			}
			else
			{
				(iterator) = children.erase(iterator); //If its not alive/getting destroyed it is erased from the vector
			}
		}
	}

	const std::string& GameObject::GetName() const
	{
		return name;
	}

	void GameObject::SetName(const std::string& newName)
	{
		name = newName;
	}

	GameObject* GameObject::GetParent()
	{
		return parent;
	}

	bool GameObject::GetIsAlive() const
	{
		return bIsAlive;
	}

	void GameObject::MarkForDestroy()
	{
		bIsAlive = false;
	}

	void GameObject::AddComponent(Component* component)
	{
		components.emplace_back(component);
		component->owner = this;
	}

	const glm::vec3& GameObject::GetPosition() const
	{
		return position;
	}

	const glm::vec3 GameObject::GetWorldPosition() const
	{
		glm::vec4 hom = GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //Homogenous coordinates
		return glm::vec3(hom) / hom.w;
	}

	void GameObject::SetPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	const glm::quat& GameObject::GetRotation() const
	{
		return rotation;
	}

	void GameObject::SetRotation(const glm::quat& rot)
	{
		rotation = rot;
	}

	const glm::vec3& GameObject::GetScale() const
	{
		return scale;
	}

	void GameObject::SetScale(const glm::vec3& scal)
	{
		scale = scal;
	}

	glm::mat4 GameObject::GetLocalTransform() const
	{
		glm::mat4 matrix = glm::mat4(1.0f); //Creates identity matrix

		//Translation
		matrix = glm::translate(matrix, position);

		//Rotation
		matrix = matrix * glm::mat4_cast(rotation); //Converts quaternion into rotation matrix
		
		//Scale
		matrix = glm::scale(matrix, scale);

		return matrix;
	}

	glm::mat4 GameObject::GetWorldTransform() const
	{
		if (parent)
		{
			return parent->GetWorldTransform() * GetLocalTransform(); //Keeps calling world transforms for parents until it reaches the top parent
		}
		else return GetLocalTransform();
	}

	void ParseGLTFNode(cgltf_node* node, GameObject* parent, const std::filesystem::path& folder)
	{
		auto object = parent->GetScene()->CreateObject(node->name, parent);

		if (node->has_matrix) //checks if that node contains a matrix of data/information including translations, scales, etc
		{
			auto mat = glm::make_mat4(node->matrix);
			glm::vec3 translation, scale, skew;
			glm::vec4 perspective;
			glm::quat orientation;
			glm::decompose(mat, scale, orientation, translation, skew, perspective);

			object->SetPosition(translation);
			object->SetRotation(orientation);
			object->SetScale(scale);
		}
		else //If it doesnt then it may contain one or few bits of data (may have translation, scale, or neither)
		{
			if (node->has_translation)
			{
				object->SetPosition(glm::vec3(node->translation[0],
					node->translation[1],
					node->translation[2]));
			}

			if (node->has_rotation)
			{
				object->SetRotation(glm::quat(node->rotation[3],
					node->rotation[0],
					node->rotation[1],
					node->rotation[2]));
			}

			if (node->has_scale)
			{
				object->SetScale(glm::vec3(node->scale[0],
					node->scale[1],
					node->scale[2]));
			}
		}

		if (node->mesh)
		{
			for (cgltf_size primIndex = 0; primIndex < node->mesh->primitives_count; primIndex++)
			{
				auto& primitive = node->mesh->primitives[primIndex];

				if (primitive.type != cgltf_primitive_type_triangles)
				{
					continue;
				}

				auto readFloats = [](const cgltf_accessor* acc, cgltf_size i, float* out, int n)
					{
						std::fill(out, out + n, 0.0f);
						return cgltf_accessor_read_float(acc, i, out, n) == 1;
					};

				auto readIndex = [](const cgltf_accessor* acc, cgltf_size i)
					{
						cgltf_uint out = 0;
						cgltf_bool ok = cgltf_accessor_read_uint(acc, i, &out, 1);
						return ok ? static_cast<uint32_t>(out) : 0;
					};

				VertexLayout vertexLayout;
				cgltf_accessor* accessors[4] = { nullptr, nullptr, nullptr, nullptr }; //4 Accessors, pos,color,uv, normal

				for (cgltf_size attIndex = 0; attIndex < primitive.attributes_count; attIndex++)
				{
					auto& attr = primitive.attributes[attIndex];
					auto acc = attr.data;

					if (!acc)
					{
						continue;
					}

					VertexElement vertexElement;
					vertexElement.type = GL_FLOAT;

					switch (attr.type) //Goes through our 3 attribute types
					{
					case cgltf_attribute_type_position:
					{
						accessors[VertexElement::PositionIndex] = acc;
						vertexElement.index = VertexElement::PositionIndex;
						vertexElement.size = 3;
					}
					break;
					case cgltf_attribute_type_color:
					{
						if (attr.index != 0)
						{
							continue;
						}
						accessors[VertexElement::ColourIndex] = acc;
						vertexElement.index = VertexElement::ColourIndex;
						vertexElement.size = 3;
					}
					break;
					case cgltf_attribute_type_texcoord:
					{
						if (attr.index != 0)
						{
							continue;
						}
						accessors[VertexElement::UVIndex] = acc;
						vertexElement.index = VertexElement::UVIndex;
						vertexElement.size = 2;
					}
					break;

					case cgltf_attribute_type_normal:
					{
						if (attr.index != 0)
						{
							continue;
						}
						accessors[VertexElement::NormalIndex] = acc;
						vertexElement.index = VertexElement::NormalIndex;
						vertexElement.size = 3;
					}
					break;
					default:
						continue;
					}

					if (vertexElement.size > 0)
					{
						vertexElement.offset = vertexLayout.stride;
						vertexLayout.stride += vertexElement.size * sizeof(float);
						vertexLayout.elements.push_back(vertexElement);
					}

				}

				if (!accessors[VertexElement::PositionIndex])
				{
					continue;
				}
				auto vertexCount = accessors[VertexElement::PositionIndex]->count;

				std::vector<float> vertices;
				vertices.resize((vertexLayout.stride / sizeof(float))* vertexCount);

				for (cgltf_size vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
				{
					for (auto& element : vertexLayout.elements)
					{
						if (!accessors[element.index])
						{
							continue;
						}

						auto index = (vertexIndex * vertexLayout.stride + element.offset) / sizeof(float);
						float* outData = &vertices[index];
						readFloats(accessors[element.index], vertexIndex, outData, element.size);
					}
				}

				std::shared_ptr<Mesh> mesh;
				if (primitive.indices)
				{
					auto indexCount = primitive.indices->count;
					std::vector<uint32_t> indices(indexCount);
					for (cgltf_size i = 0; i < indexCount; i++)
					{
						indices[i] = readIndex(primitive.indices, i);
					}
					mesh = std::make_shared<Mesh>(vertexLayout, vertices, indices);
				}
				else
				{
					mesh = std::make_shared<Mesh>(vertexLayout, vertices);
				}

			}
		}
	}

	GameObject* GameObject::LoadGLTF(const std::string& path)
	{
		auto contents = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
		if (contents.empty())
		{
			return nullptr;
		}

		cgltf_options options = {};
		cgltf_data* data = nullptr;

		cgltf_result res = cgltf_parse(&options, contents.data(), contents.size(), &data);
		if (res != cgltf_result_success)
		{
			return nullptr;
		}

		auto fullPath = Engine::GetInstance().GetFileSystem().GetAssetsFolder() / path;
		auto fullFolderPath = fullPath.remove_filename();
		auto relativeFolderPath = std::filesystem::path(path).remove_filename();
		res = cgltf_load_buffers(&options, data, fullFolderPath.string().c_str());
		if (res != cgltf_result_success)
		{
			cgltf_free(data);
			return nullptr;
		}

		auto resultObject = Engine::GetInstance().GetScene()->CreateObject("Result");
		auto scene = &data->scenes[0];

		for (cgltf_size i = 0; i < scene->nodes_count; i++)
		{
			auto node = scene->nodes[i];
			ParseGLTFNode(node, resultObject, relativeFolderPath);
		}
	}
}