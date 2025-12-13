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
		void SetParam(const std::string& name, float v1, float v2);
		void Bind();

	private:
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::unordered_map<std::string, float> floatParams;
		std::unordered_map<std::string, std::pair<float, float>> float2Params;
	};
}