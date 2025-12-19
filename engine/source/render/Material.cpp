#include "render/Material.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"

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
	}
}