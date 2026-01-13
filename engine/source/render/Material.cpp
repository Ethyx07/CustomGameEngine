#include "render/Material.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"
#include "Engine.h"
#include "nlohmann/json.hpp"

namespace eng
{
	void Material::SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProg)
	{
		shaderProgram = shaderProg; //Sets the shader program for this material to the parameter
	}

	ShaderProgram* Material::GetShaderProgram()
	{
		return shaderProgram.get();
	}


	void Material::SetParam(const std::string& name, float value)
	{
		floatParams[name] = value; //Stores the name of the uniform and the value of it (only for 1 float uniforms)
	}

	void Material::SetParam(const std::string& name, float v1, float v2)
	{
		float2Params[name] = { v1, v2 };
	}

	void Material::SetParam(const std::string& name, glm::vec3 float3)
	{
		float3Params[name] = float3;
	}

	void Material::SetParam(const std::string& name, const std::shared_ptr<Texture>& texture)
	{
		textures[name] = texture;
	}

	void Material::Bind()
	{
		if (!shaderProgram) //Binds the shader program if it is valid
		{
			return;
		}

		shaderProgram->Bind(); 

		for (auto& param : floatParams) //Loops through the list of single float uniforms and calls set uniform with their name and value
		{
			shaderProgram->SetUniform(param.first, param.second);
		}

		for (auto& param : float2Params) //Allows for 2 parameter offsets
		{
			shaderProgram->SetUniform(param.first, param.second.first, param.second.second);
		}

		for (auto& param : textures)
		{
			shaderProgram->SetTexture(param.first, param.second.get());
		}

		for (auto& param : float3Params) //Iterates through all float3Params, setting their uniforms
		{
			shaderProgram->SetUniform(param.first, param.second);
		}
	}

	std::shared_ptr<Material> Material::Load(const std::string& path)
	{
		auto contents = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
		if (contents.empty()) //Returns null if asset file is empty or doesnt exist
		{
			return nullptr;
		}

		nlohmann::json json = nlohmann::json::parse(contents); //Parses content of material to create a dictionary of data
		std::shared_ptr<Material> result;

		if (json.contains("shader")) //Gets the collection of data under the shader key
		{
			auto shaderObj = json["shader"];
			std::string vertexPath = shaderObj.value("vertex", ""); //Gets the value with the key vertex. If vertex isnt a key returns ""
			std::string fragPath = shaderObj.value("fragment", ""); //Same as above

			auto& fs = Engine::GetInstance().GetFileSystem();
			auto vertexSource = fs.LoadAssetFileText(vertexPath); //Loads shader sources based on path
			auto fragmentSource = fs.LoadAssetFileText(fragPath);

			auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
			auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexSource, fragmentSource); //Creates shader program

			if (!shaderProgram) //If shader program fails return null (cant make material without it)
			{
				return nullptr;
			}

			result = std::make_shared<Material>(); //Creates a result material that we can alter its values
			result->SetShaderProgram(shaderProgram); //Sets the shader program to be used
		}

		if (json.contains("params")) //Checks if the material file has params
		{
			auto paramObj = json["params"];

			//Floats
			if (paramObj.contains("float")) //Loops through and applies all single float params
			{
				for (auto& param : paramObj["float"])
				{
					std::string name = param.value("name", "");
					float value = param.value("value", 0.0f);
					result->SetParam(name, value);
				}
			}

			//Float2
			if (paramObj.contains("float2")) //Loops through and applies all double float params
			{
				for (auto& param : paramObj["float2"])
				{
					std::string name = param.value("name", "");
					float v1 = param.value("value1", 0.0f);
					float v2 = param.value("value2", 0.0f);
					result->SetParam(name, v1, v2);
				}
			}

			//Textures
			if (paramObj.contains("textures")) //Loops through, creates and applies all texture params
			{
				for (auto& param : paramObj["textures"])
				{
					std::string name = param.value("name", "");
					std::string texturePath = param.value("path", "");
					auto texture = Texture::Load(texturePath);
					result->SetParam(name, texture);
				}
			}
		}
		return result;
	}
}