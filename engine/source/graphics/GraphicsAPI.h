#pragma once
#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>

namespace eng
{
	class ShaderProgram;
	class Material;
	class Mesh;

	enum class BlendMode
	{
		Disabled,
		Alpha,
		Additive,
		Multiply
	};

	struct Rect
	{
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};

	class GraphicsAPI
	{
	public:
		bool Init();
		void SetViewport(int x, int y, int width, int height);
		const Rect& GetViewport() const;

		std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
		const std::shared_ptr<ShaderProgram>& GetDefaultShaderProgram();
		const std::shared_ptr<ShaderProgram>& GetDefault2DShaderProgram();
		const std::shared_ptr<ShaderProgram> & GetDefaultUIShaderProgram();
		GLuint CreateVertexBuffer(const std::vector<float>& vertices);
		GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);

		void BindShaderProgram(ShaderProgram* shaderProgram);
		void BindMaterial(Material* material);

		void SetClearColour(float r, float g, float b, float a);
		void ClearBuffers();

		void SetDepthTestEnabled(bool enabled);
		void SetBlendMode(BlendMode mode);

		void BindMesh(Mesh* mesh);
		void UnbindMesh(Mesh* mesh);
		void DrawMesh(Mesh* mesh);

	private:
		std::shared_ptr<ShaderProgram> defaultShaderProgram;
		std::shared_ptr<ShaderProgram> default2DShaderProgram;
		std::shared_ptr<ShaderProgram> defaultUIShaderProgram;

		Rect viewport;
	};
}