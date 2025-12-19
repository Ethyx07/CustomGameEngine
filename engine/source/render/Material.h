#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace eng
{
	class ShaderProgram;
	class Texture;

	class Material
	{
	public:
		void SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProg);
		ShaderProgram* GetShaderProgram();

		void SetParam(const std::string& name, float value);
		void SetParam(const std::string& name, float v1, float v2);
		void SetParam(const std::string& name, const std::shared_ptr<Texture>& texture);
		void Bind();

		
	private:
		std::shared_ptr<ShaderProgram> shaderProgram;
		std::unordered_map<std::string, float> floatParams;
		std::unordered_map<std::string, std::pair<float, float>> float2Params;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}