#include <graphics/GraphicsAPI.h>
#include <graphics/ShaderProgram.h>
#include <iostream>

namespace eng
{
	std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexSource,
		const std::string& fragmentSource)
	{
        //Creates and compiles vertex shader, checks if if was compiled properly //
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexShaderCStr = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR:VERTEX SHADER COMPILATION FAILED: " << infoLog << std::endl;
            return nullptr;
        }


        //Compiles fragmentShader and checks if it compiled properly //
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentShaderCStr = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR: FRAGMENT SHADER COMPILATION FAILED: " << infoLog << std::endl;
            return nullptr;
        }
        //Creates shader program and checks if it properly linked
        GLuint shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);
        glLinkProgram(shaderProgramID);

        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
            std::cerr << "ERROR: SHADER PROGRAM LINK FAILED: " << infoLog << std::endl;
            return nullptr;
        }
        //Deletes shaders as they are now internally stored after the link
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return std::make_shared<ShaderProgram>(shaderProgramID);
	}

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        shaderProgram->Bind();
    }

}