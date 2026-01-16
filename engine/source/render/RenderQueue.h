#pragma once
#include "Common.h"
#include <vector>
#include <glm/mat4x4.hpp>
#include <memory>


namespace eng
{

	class Texture;
	class Mesh;
	class Material;
	class GraphicsAPI;

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

	class RenderQueue
	{
	public:
		void Init();
		void Submit(const RenderCommand& command);
		void Submit(const RenderCommand2D& command);
		void Draw(GraphicsAPI& graphicsAPI, const CameraData& cameraData, const std::vector<LightData> lights);

	private:
		std::vector<RenderCommand> commandList;
		std::vector<RenderCommand2D> commandList2D;
		std::shared_ptr<Mesh> mesh2D;
	};
}