#include "graphics/GraphicsAPI.h"
#include "graphics/ShaderProgram.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include <iostream>
namespace eng
{
    bool GraphicsAPI::Init()
    {
        glEnable(GL_DEPTH_TEST);
        return true;
    }

    void GraphicsAPI::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
    }

    const Rect& GraphicsAPI::GetViewport() const
    {
        return viewport;
    }

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

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefaultShaderProgram()
    {
        if (!defaultShaderProgram)
        {
            std::string vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 color;
            layout (location = 2) in vec2 uv;
            layout (location = 3) in vec3 normal;

            out vec2 vUV;
            out vec3 vNormal;
            out vec3 vFragPos;

            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;

            void main()
            {
                vUV = uv;

                vFragPos = vec3(uModel * vec4(position, 1.0));

                vNormal = mat3(transpose(inverse(uModel))) * normal;
                gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
            }
            )";

            std::string fragmentShaderSource = R"(
            #version 330 core

            struct Light
            {
                vec3 colour;
                vec3 direction;
            };
            uniform Light uLight;
            uniform vec3 uCameraPos;
            out vec4 FragColor;

            in vec2 vUV;
            in vec3 vNormal;
            in vec3 vFragPos;

            uniform sampler2D baseColourTexture;

            void main()
            {
                vec3 norm = normalize(vNormal);

                //Diffuse Calc
                vec3 lightDir = normalize(-uLight.direction);
                float diff = max(dot(norm, lightDir), 0);
                vec3 diffuse = diff * uLight.colour;

                //Specular Calc
                vec3 viewDir = normalize(uCameraPos - vFragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
                float specularStrength = 0.5;
                vec3 specular = specularStrength * spec * uLight.colour;

                //Ambient Calc
                const float ambientStrength = 0.4;
                vec3 ambient = ambientStrength * uLight.colour;

                vec3 result = diffuse + specular + ambient;

                vec4 texColor = texture(baseColourTexture, vUV);
                FragColor = texColor * vec4(result, 1.0);
            }
            )";

            defaultShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }
        return defaultShaderProgram;
    }

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefault2DShaderProgram()
    {
        if (!default2DShaderProgram) //Ensures we are only creating it before returning it if it is yet to be created
        {
            std::string vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec2 position;
            
            out vec2 vUV;
            
            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;

            uniform vec2 uPivot;
            uniform vec2 uSize;

            uniform vec2 uUVMin;
            uniform vec2 uUVMax;

            void main()
            {
                vec2 local = (position - uPivot) * uSize;
                vUV = mix(uUVMin, uUVMax, position);
        
                gl_Position = uProjection * uView * uModel * vec4(local, 0.0, 1.0);
            }
            )";

            std::string fragmentShaderSource = R"(
            #version 330 core

            in vec2 vUV;

            uniform vec4 uColour;

            uniform sampler2D uTex;

            out vec4 FragColour;

            void main()
            {
                vec4 source = texture(uTex, vUV) * uColour;
                FragColour = source;
            }
            )";

            default2DShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }

        return default2DShaderProgram;
    }

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefaultUIShaderProgram()
    {
        if (!defaultUIShaderProgram)
        {
            std::string vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec2 position;
            layout (location = 1) in vec4 colour;
            layout (location = 2) in vec2 uv;

            out vec2 vUV;
            out vec4 vColour;

            uniform mat4 uProjection;

            void main()
            {
                vUV = uv;
                vColour = colour;
    
                gl_Position = uProjection * vec4(position,0.0, 1.0);
            }
            )";

            //src checks if use texture != 0. If true then multiply texture and colour, if false just use colour
            std::string fragmentShaderSource = R"(
            #version 330 core

            in vec2 vUV;
            in vec4 vColour;

            uniform sampler2D uTex;
            uniform int uUseTexture;

            out vec4 FragColor;

            void main()
            {
                vec4 src = (uUseTexture != 0) ? texture(uTex, vUV) * vColour : vColour;
                FragColor = src;
            }
            )";

            defaultUIShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }

        return defaultUIShaderProgram;
    }

    GLuint GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vertices)
    {
        GLuint VBO = 0;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO); //ARRAY_BUFFER tells that it will store an array of data
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinds buffer but vertex data is already stored
        return VBO;
    }

    GLuint GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
    {
        GLuint EBO = 0;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //ARRAY_BUFFER tells that it will store an array of data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbinds buffer but vertex data is already stored
        return EBO;
    }

    void GraphicsAPI::SetClearColour(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void GraphicsAPI::ClearBuffers()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void GraphicsAPI::SetDepthTestEnabled(bool enabled)
    {
        if (enabled)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void GraphicsAPI::SetBlendMode(BlendMode mode)
    {
        switch (mode)
        {
        case BlendMode::Disabled: 
        {
            glDisable(GL_BLEND);
        }
            break;
        case BlendMode::Alpha:
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
            break;
        case BlendMode::Additive:
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
        }
            break;
        case BlendMode::Multiply:
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
        }
            break;
        default:
        {
            glDisable(GL_BLEND);
        }
            break;
        }
    }

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        if (shaderProgram) //Binds our shader program if it is valid
        {
            shaderProgram->Bind();
        }
        
    }

    void  GraphicsAPI::BindMaterial(Material* material)
    {
        if (material) //Binds our material if it is valid
        {
            material->Bind();
        }
    }

    void  GraphicsAPI::BindMesh(Mesh* mesh)
    {
        if (mesh) //Binds mesh if mesh is valid
        {
            mesh->Bind();
        }
    }

    void GraphicsAPI::UnbindMesh(Mesh* mesh)
    {
        if (mesh)
        {
            mesh->Unbind();
        }
    }

    void  GraphicsAPI::DrawMesh(Mesh* mesh)
    {
        if (mesh) //Draws mesh if mesh is valid
        {
            mesh->Draw();
        }
    }

}