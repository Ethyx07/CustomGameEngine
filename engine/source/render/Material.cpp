#include "render/Material.h"
#include "graphics/ShaderProgram.h"

namespace eng
{
	void Material::SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProg)
	{
		shaderProgram = shaderProg; //Sets the shader program for this material to the parameter
	}

	void Material::SetParam(const std::string& name, float value)
	{
		floatParams[name] = value; //Stores the name of the uniform and the value of it (only for 1 float uniforms)
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
	}
}