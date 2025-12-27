#include "render/Mesh.h"
#include "graphics/GraphicsAPI.h"
#include "Engine.h"


#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

namespace eng
{
	Mesh::Mesh(const VertexLayout& layout, const std::vector<float> vertices, const std::vector<uint32_t> indices)
	{
		vertexLayout = layout;

		auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();

		VBO = graphicsAPI.CreateVertexBuffer(vertices);
		EBO = graphicsAPI.CreateIndexBuffer(indices);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		for (auto& element : vertexLayout.elements)
		{
			glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
				vertexLayout.stride, (void*)(uintptr_t)element.offset);
			glEnableVertexAttribArray(element.index);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		vertexCount = (vertices.size() * sizeof(float)) / vertexLayout.stride;
		indexCount = indices.size();
	}

	Mesh::Mesh(const VertexLayout& layout, const std::vector<float> vertices)
	{
		vertexLayout = layout;

		auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();

		VBO = graphicsAPI.CreateVertexBuffer(vertices);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		for (auto& element : vertexLayout.elements)
		{
			glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
				vertexLayout.stride, (void*)(uintptr_t)element.offset);
			glEnableVertexAttribArray(element.index);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		vertexCount = (vertices.size() * sizeof(float)) / vertexLayout.stride;
	}

	void Mesh::Bind()
	{
		glBindVertexArray(VAO);
	}

	void Mesh::Draw()
	{
		if (indexCount > 0) //Checks if mesh was made with indices (EBO would be created then). Draws elements if there are indices and draws array (VAO) if not
		{
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		}
		else 
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}

	std::shared_ptr<Mesh> Mesh::CreateCube()
	{
		std::vector<float> vertices =
		{
			// Front face
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f,

			// Top face 
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,

			// Right face
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			// Left face
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			// Bottom face
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, -1.0f, 0.0f,

			// Back face
			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		};

		std::vector<unsigned int> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,
			// top face
			4, 5, 6,
			4, 6, 7,
			// right face
			8, 9, 10,
			8, 10, 11,
			// left face
			12, 13, 14,
			12, 14, 15,
			// bottom face
			16, 17, 18,
			16, 18, 19,
			// back face
			20, 21, 22,
			20, 22, 23
		};

		eng::VertexLayout vertexLayout;

		// Postion
		vertexLayout.elements.push_back({
			VertexElement::PositionIndex,
			3,
			GL_FLOAT,
			0
			});
		// Color
		vertexLayout.elements.push_back({
			VertexElement::ColourIndex,
			3,
			GL_FLOAT,
			sizeof(float) * 3
			});
		// UV
		vertexLayout.elements.push_back({
			VertexElement::UVIndex,
			2,
			GL_FLOAT,
			sizeof(float) * 6
			});
		//Normal
		vertexLayout.elements.push_back({
			VertexElement::NormalIndex,
			3,
			GL_FLOAT,
			sizeof(float) * 8
			});

		vertexLayout.stride = sizeof(float) * 11;

		auto result = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices);

		return result;
	}

	std::shared_ptr<Mesh> Mesh::Load(const std::string& path)
	{
		auto contents = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);

		if (contents.empty())
		{
			return nullptr;
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

		cgltf_options options = {};
		cgltf_data* data = nullptr;

		cgltf_result res = cgltf_parse(&options, contents.data(), contents.size(), &data);
		if (res != cgltf_result_success)
		{
			return nullptr;
		}

		auto fullPath = Engine::GetInstance().GetFileSystem().GetAssetsFolder() / path;
		res = cgltf_load_buffers(&options, data, fullPath.remove_filename().string().c_str());
		if (res != cgltf_result_success)
		{
			cgltf_free(data);
			return nullptr;
		}

		std::shared_ptr<Mesh> result = nullptr;

		for (cgltf_size meshIndex = 0; meshIndex < data->meshes_count; meshIndex++) //In our mesh count finds the first primitive triangle and builds from that
		{
			auto& mesh = data->meshes[meshIndex];
			for (cgltf_size primativeIndex = 0; primativeIndex < mesh.primitives_count; primativeIndex++)
			{
				auto& primitive = mesh.primitives[primativeIndex];
				if (primitive.type != cgltf_primitive_type_triangles)
				{
					continue;
				}
				
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
				vertices.resize((vertexLayout.stride / sizeof(float)) * vertexCount);

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

				if (primitive.indices)
				{
					auto indexCount = primitive.indices->count;
					std::vector<uint32_t> indices(indexCount);
					for (cgltf_size i = 0; i < indexCount; i++)
					{
						indices[i] = readIndex(primitive.indices, i);
					}
					result = std::make_shared<Mesh>(vertexLayout, vertices, indices);
				}
				else
				{
					result = std::make_shared<Mesh>(vertexLayout, vertices);
				}

				if (result)
				{
					break;
				}

			}
			if (result)
			{
				break;
			}
		}
		cgltf_free(data);
		return result;
	}
}