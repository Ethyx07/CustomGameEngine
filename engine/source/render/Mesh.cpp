#include "render/Mesh.h"
#include "graphics/GraphicsAPI.h"
#include "Engine.h"



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

	void Mesh::Unbind()
	{
		glBindVertexArray(0);
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

	std::shared_ptr<Mesh> Mesh::CreateBox(const glm::vec3& extents)
	{
		const glm::vec3 half = extents * 0.5f;
		std::vector<float> vertices =
		{
			// Front face
		   half.x, half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		   -half.x, half.y, half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		   -half.x, -half.y, half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		   half.x, -half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		   // Top face 
		   half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		   -half.x, half.y, -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		   -half.x, half.y, half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		   half.x, half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		   // Right face
		   half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		   half.x, half.y, half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		   half.x, -half.y, half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		   half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		   // Left face
		   -half.x, half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		   -half.x, half.y, -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		   -half.x, -half.y, -half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		   -half.x, -half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		   // Bottom face
		   half.x, -half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		   -half.x, -half.y, half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		   -half.x, -half.y, -half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		   half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

		   // Back face
		   -half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		   half.x, half.y, -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		   half.x, -half.y, -half.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		   -half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f
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
}