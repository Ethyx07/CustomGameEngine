#pragma once
#include <GL/glew.h>
#include "graphics/VertexLayout.h"
#include <glm/vec3.hpp>
#include <memory>
#include <string>


namespace eng
{
	class Mesh
	{
	public:
		Mesh(const VertexLayout& layout, const std::vector<float> vertices, const std::vector<uint32_t> indices);
		Mesh(const VertexLayout& layout, const std::vector<float> vertices);
		Mesh(const Mesh&) = delete;
		Mesh& operator = (const Mesh&) = delete;

		void Bind();
		void Unbind();
		void Draw();

		static std::shared_ptr<Mesh> CreateBox(const glm::vec3& extents = glm::vec3(1.0f));
		static std::shared_ptr<Mesh> Mesh::CreateSphere(float radius, int sectors, int stacks);
		//static std::shared_ptr<Mesh> Load(const std::string& path);

	private:
		VertexLayout vertexLayout;
		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		size_t vertexCount = 0;
		size_t indexCount = 0;
	};
}