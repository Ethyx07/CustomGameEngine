#pragma once
#include "Common.h"
#include <vector>
#include <glm/mat4x4.hpp>
#include <memory>


namespace eng
{

	class Mesh;
	class Material;
	class GraphicsAPI;
	class Texture;
	class ShaderProgram;

	struct RenderCommand
	{
		Mesh* mesh = nullptr;
		Material* material = nullptr;
		glm::mat4 modelMatrix;
	};

	struct RenderCommand2D
	{
		glm::mat4 modelMatrix;
		Texture* texture = nullptr;
		glm::vec4 colour;
		glm::vec2 size;
		glm::vec2 lowerLeftUV;
		glm::vec2 upperRightUV;
		glm::vec2 pivot;
	};

	struct RenderCommandUI
	{
		Mesh* mesh;
		ShaderProgram* shaderProgram;
		size_t screenWidth = 0;
		size_t screenHeight = 0;
		std::vector<UIBatch> batches;
	};

	class RenderQueue
	{
	public:
		void Init();
		void Submit(const RenderCommand& command);
		void Submit(const RenderCommand2D& command);
		void Submit(const RenderCommandUI& command);
		void Draw(GraphicsAPI& graphicsAPI, const CameraData& cameraData, const std::vector<LightData> lights);

	private:
		std::vector<RenderCommand> commandList;
		std::vector<RenderCommand2D> commandList2D;
		std::shared_ptr<Mesh> mesh2D;
		std::vector<RenderCommandUI> commandListUI;
	};
}