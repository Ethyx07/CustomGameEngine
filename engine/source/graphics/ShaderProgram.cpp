#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"
#include "glm/gtc/type_ptr.hpp"

namespace eng
{
	ShaderProgram::ShaderProgram(GLuint shaderProgID) : shaderProgramID(shaderProgID)
	{

	}

	ShaderProgram::~ShaderProgram() //Removes shader program from memory. Destructor does it intentionally to prevent double destroying
	{
		glDeleteProgram(shaderProgramID);
	}

	void ShaderProgram::Bind() //Binds shader program as current program to use
	{
		glUseProgram(shaderProgramID);
		currentTextureUnit = 0;
	}

	GLint ShaderProgram::GetUniformLocation(const std::string& name)
	{
		auto iterator = uniformLocationCache.find(name); //Checks if cache contains name
		if (iterator != uniformLocationCache.end()) //If iterator == end then its not in the cache
		{
			return iterator->second; //Returns the location
		}
		GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
		uniformLocationCache[name] = location; //Adds to cache and sets location
		return location;
	}

	void ShaderProgram::SetUniform(const std::string& name, float value) //One version of the setuniform function for when uniform is 1 float
	{
		auto location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void ShaderProgram::SetUniform(const std::string& name, float v1, float v2)
	{
		auto location = GetUniformLocation(name);
		glUniform2f(location, v1, v2);
	}

	void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& matrix)
	{
		auto location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
	{
		auto location = GetUniformLocation(name);
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void ShaderProgram::SetTexture(const std::string& name, Texture* texture)
	{

		auto location = GetUniformLocation(name);

		glActiveTexture(GL_TEXTURE0 + currentTextureUnit);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());
		glUniform1i(location, currentTextureUnit);
		currentTextureUnit += 1;
	}
}