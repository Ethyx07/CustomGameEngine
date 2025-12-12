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

	void Mesh::Draw()
	{
		if (indexCount > 0) 
		{
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		}
		else 
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}
}