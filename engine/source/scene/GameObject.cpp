#include "scene/GameObject.h"
#include "Engine.h"
#include "graphics/VertexLayout.h"
#include "graphics/Texture.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "scene/components/MeshComponent.h"
#include "components/AnimationComponent.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>


namespace eng
{
	void GameObject::LoadProperties(const nlohmann::json& json)
	{
	}

	void GameObject::Init()
	{
	}

	void GameObject::Update(float deltaTime)
	{
		if (!bActive)
		{
			return;
		}

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

	bool GameObject::SetParent(GameObject* parent)
	{
		if (!scene)
		{
			return false;
		}

		return scene->SetParent(this, parent);
	}

	bool GameObject::IsActive()
	{
		return bActive;
	}

	void GameObject::SetActive(bool active)
	{
		bActive = active;
	}

	GameObject* GameObject::FindChildByName(const std::string& childName) //Recursive function that checks all children of the initial gameobject
	{
		if (name == childName) //Returns this if the childname is this object
		{
			return this;
		}
		for (auto& child : children) //Checks the children of this object until it finds the name or checks all children
		{
			if (auto result = child->FindChildByName(childName))
			{
				return result;
			}
		}
		return nullptr;
	}

	Scene* GameObject::GetScene()
	{
		return scene;
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
		component->Init(); //If component uses the init function (physics) runs its initialisation logic
	}

	const glm::vec3& GameObject::GetPosition() const
	{
		return position;
	}

	void GameObject::SetPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	const glm::vec3 GameObject::GetWorldPosition() const
	{
		glm::vec4 hom = GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //Homogenous coordinates
		return glm::vec3(hom) / hom.w;
	}

	void GameObject::SetWorldPosition(const glm::vec3& pos)
	{
		if (parent) //If there is a parent it calcs its world position based off of the parent, else sets pos as pos
		{
			glm::mat4 parentWorld = parent->GetWorldTransform();
			glm::mat4 invParentWorld = glm::inverse(parentWorld);
			glm::vec4 localPos = invParentWorld * glm::vec4(pos, 1.0f);
			SetPosition(glm::vec3(localPos) / localPos.w);
		}
		else
		{
			SetPosition(pos);
		}
	}

	const glm::quat& GameObject::GetRotation() const
	{
		return rotation;
	}

	void GameObject::SetRotation(const glm::quat& rot)
	{
		rotation = rot;
	}

	const glm::quat GameObject::GetWorldRotation() const
	{
		if (parent) //If there is a parent it keeps returning that parent's rotation * its own. Repeats until object has no parent
		{
			return parent->GetWorldRotation() * rotation;
		}
		else
		{
			return rotation;
		}
	}

	void GameObject::SetWorldRotation(const glm::quat& rot)
	{
		if (parent)
		{
			glm::quat parentWorldRot = parent->GetWorldRotation();
			glm::quat invParentWorldRot = glm::inverse(parentWorldRot);
			glm::quat newLocalRot = invParentWorldRot * rot;
			SetRotation(newLocalRot);
		}
		else
		{
			SetRotation(rot);
		}
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

		if (node->mesh) //Checks if node is a mesh and if so gets its primatives and creates the mesh from that (only triangles)
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
				if (primitive.indices) //If the primitive has a indices array it goes through that too and creates our mesh using the indices constructor
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

				auto mat = std::make_shared<Material>();
				mat->SetShaderProgram(Engine::GetInstance().GetGraphicsAPI().GetDefaultShaderProgram()); //Sets material to default shader program in case primitive has none

				if (primitive.material) 
				{
					auto gltfMat = primitive.material;
					if (gltfMat->has_pbr_metallic_roughness) //Checks if primitive has this pbr type
					{
						auto pbr = gltfMat->pbr_metallic_roughness;
						auto texture = pbr.base_color_texture.texture;
						if (texture && texture->image) //Makes sure texture is valid and contains an image
						{
							if (texture->image->uri)
							{
								auto path = folder / std::string(texture->image->uri);
								auto tex = Engine::GetInstance().GetTextureManager().GetOrLoadTexture(path.string()); //Loads texture or returns already loaded texture
								mat->SetParam("baseColourTexture", tex); //Applies texture as shader material baseColourTexture
							}
						}
					}
					else if (gltfMat->has_pbr_specular_glossiness) //Does the same as above for specular
					{
						auto pbr = gltfMat->pbr_specular_glossiness;
						auto texture = pbr.diffuse_texture.texture;
						if (texture && texture->image)
						{
							if (texture->image->uri)
							{
								auto path = folder / std::string(texture->image->uri);
								auto tex = Engine::GetInstance().GetTextureManager().GetOrLoadTexture(path.string());
								mat->SetParam("baseColourTexture", tex);
							}
						}
					}

					object->AddComponent(new MeshComponent(mat, mesh)); //Adds component to the overall game object
				}
			}
		}

		for (cgltf_size childIndex = 0; childIndex < node->children_count; childIndex++) //Loops through all children of this node and parses those nodes too
		{
			ParseGLTFNode(node->children[childIndex], object, folder);
		}
	}


	static float ReadScalar(const cgltf_accessor* acc, cgltf_size index)
	{
		float result = 0.0f;
		cgltf_accessor_read_float(acc, index, &result, 1);
		return result;
	}

	static glm::vec3 ReadVec3(const cgltf_accessor* acc, cgltf_size index)
	{
		glm::vec3 result;
		cgltf_accessor_read_float(acc, index, glm::value_ptr(result), 3);
		return result;
	}

	static glm::quat ReadQuat(const cgltf_accessor* acc, cgltf_size index)
	{
		float temp[4] = { 0,0,0,1 }; //x, y, z, w
		cgltf_accessor_read_float(acc, index, temp, 4);
		//glm quats are w, x, y, z but temp[4] is x, y, z, w
		return glm::quat(temp[3], temp[0], temp[1], temp[2]);

	}

	static void ReadTimes(const cgltf_accessor* acc, std::vector<float>& outTimes)
	{
		outTimes.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; i++)
		{
			outTimes[i] = ReadScalar(acc, i); 
		}
	}

	static void ReadOutputVec3(const cgltf_accessor* acc, std::vector<glm::vec3>& outValues)
	{
		outValues.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; i++)
		{
			outValues[i] = ReadVec3(acc, i); 
		}
	}

	static void ReadOutputQuat(const cgltf_accessor* acc, std::vector<glm::quat>& outValues)
	{
		outValues.resize(acc->count);
		for (cgltf_size i = 0; i < acc->count; i++)
		{
			outValues[i] = ReadQuat(acc, i); 
		}
	}

	GameObject* GameObject::LoadGLTF(const std::string& path) //Loads GLTF from path. Allows for GLTF scenes that contain multiple objects/meshes to be loaded as one with their hierarchy intact
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

		std::vector<std::shared_ptr<AnimationClip>> clips; //Collection of clips/animations within the gltf data
		for (cgltf_size animIndex = 0; animIndex < data->animations_count; animIndex++)
		{
			auto& animation = data->animations[animIndex];

			auto clip = std::make_shared<AnimationClip>(); //Creates a clip to pass the gltf data into
			clip->name = animation.name ? animation.name : "noname"; //Sets anim name to the clips name if one is found
			clip->duration = 0.0f;

			std::unordered_map<cgltf_node*, size_t> trackIndexOf;

			auto GetOrCreateTrack = [&](cgltf_node* node) -> TransformTrack&
				{
					auto iterator = trackIndexOf.find(node); //Checks if the track is already stored/created
					if (iterator != trackIndexOf.end())
					{
						return clip->tracks[iterator->second];
					}

					TransformTrack track;
					track.targetName = node && node->name ? node->name : std::string{}; //If node and node name are valid then sets target to node name
					clip->tracks.push_back(track);
					size_t idx = clip->tracks.size() - 1;
					trackIndexOf[node] = idx; //Sets the nodes index value to align with the tracks index in the track vector
					return clip->tracks[idx];
				};

			for (cgltf_size clipIndex = 0; clipIndex < animation.channels_count; clipIndex++) //Loops through all channels of this clip/anmation
			{
				auto& channel = animation.channels[clipIndex]; //Gets the current channel
				auto sampler = channel.sampler; //Gets the sampler specific to that channel

				if (!channel.target_node || !sampler || !sampler->input || !sampler->output) //If any of these are invalid, skip to next index in loop
				{
					continue;
				}

				std::vector<float> times;
				ReadTimes(sampler->input, times); //Takes the sampler input as its accessor and the times vector as its outValues

				if (times.empty()) //If no time values read, skip to next index
				{
					continue;
				}

				auto& track = GetOrCreateTrack(channel.target_node); //Returns the track from this channel using the target node

				switch (channel.target_path) //The type of animation/change
				{
				case cgltf_animation_path_type_translation:
				{
					std::vector<glm::vec3> values;
					ReadOutputVec3(sampler->output, values); //Passes sampler output as accessor
					track.positionKeys.resize(times.size()); //Resizes to match the size of the times vector as each time should have a corresponding key
					for (size_t i = 0; i < times.size(); i++)
                    {
                        track.positionKeys[i].time = times[i];
                        track.positionKeys[i].value = values[i];
                    }
				}
				break;

				case cgltf_animation_path_type_scale:
				{
					std::vector<glm::vec3> values;
					ReadOutputVec3(sampler->output, values); //Passes sampler output as accessor
					track.scaleKeys.resize(times.size()); //Resizes to match the size of the times vector as each time should have a corresponding key
					for (size_t i = 0; i < times.size(); i++)
					{
						track.scaleKeys[i].time = times[i];
						track.scaleKeys[i].value = values[i];
					}
				}
				break;

				case cgltf_animation_path_type_rotation:
				{
					std::vector<glm::quat> values;
					ReadOutputQuat(sampler->output, values); //Passes sampler output as accessor
					track.rotationKeys.resize(times.size()); //Resizes to match the size of the times vector as each time should have a corresponding key
					for (size_t i = 0; i < times.size(); i++)
					{
						track.rotationKeys[i].time = times[i];
						track.rotationKeys[i].value = values[i];
					}
				}
				break;
				default:
					break;
				}

				clip->duration = std::max(clip->duration, times.back()); //Gets the max value between our clip->duration and the last time value
			}

			clips.push_back(clip);
		}

		if (!clips.empty()) //If clip data had been found and stored
		{
			auto animComp = new AnimationComponent(); //Adds animation component to object and registers its clips to it
			resultObject->AddComponent(animComp);
			for (auto& clip : clips)
			{
				animComp->RegisterClip(clip->name, clip);
			}
		}

		cgltf_free(data);

		return resultObject;
	}
}