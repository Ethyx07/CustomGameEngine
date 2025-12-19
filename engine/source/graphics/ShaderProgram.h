#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

namespace eng
{
	class Texture;

	class ShaderProgram
	{
	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator = (const ShaderProgram&) = delete;
		~ShaderProgram();

		explicit ShaderProgram(GLuint shaderProgID);
		void Bind();
		GLint GetUniformLocation(const std::string& name);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, float v1, float v2);
		void SetUniform(const std::string& name, const glm::mat4& matrix);
		void SetTexture(const std::string& name, Texture* texture);

	private:
		std::unordered_map<std::string, GLint> uniformLocationCache;
		GLuint shaderProgramID = 0;
	};
}