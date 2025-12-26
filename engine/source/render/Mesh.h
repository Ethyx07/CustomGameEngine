#pragma once
#include <GL/glew.h>
#include "graphics/VertexLayout.h"
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
		void Draw();

		static std::shared_ptr<Mesh> Load(const std::string& path);

	private:
		VertexLayout vertexLayout;
		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		size_t vertexCount = 0;
		size_t indexCount = 0;
	};
}