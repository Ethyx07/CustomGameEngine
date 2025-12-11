#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace eng
{
	class ShaderProgram;

	class Material
	{
	public:
		void SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProg);
		void SetParam(const std::string& name, float value);
		void Bind();

	private:
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::unordered_map<std::string, float> floatParams;
	};
}