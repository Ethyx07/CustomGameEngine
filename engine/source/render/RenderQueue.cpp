#include "render/RenderQueue.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "graphics/GraphicsAPI.h"
#include "graphics/ShaderProgram.h"

namespace eng
{
	void RenderQueue::Submit(const RenderCommand& command)
	{
		commandList.push_back(command);  //Adds render command to list. command contains a mesh and material
	}

	void RenderQueue::Draw(GraphicsAPI& graphicsAPI, const CameraData& cameraData, const std::vector<LightData> lights)
	{
		for (auto& command : commandList)
		{
			graphicsAPI.BindMaterial(command.material); //Binds material
			auto shaderProgram = command.material->GetShaderProgram(); //Gets shader program to be used for uniform data and camera data
			shaderProgram->SetUniform("uModel", command.modelMatrix);
			shaderProgram->SetUniform("uView", cameraData.viewMatrix);
			shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);
			shaderProgram->SetUniform("uCameraPos", cameraData.position);
			if (!lights.empty())
			{
				auto& light = lights[0];
				shaderProgram->SetUniform("uLight.colour", light.colour);
				shaderProgram->SetUniform("uLight.position", light.position);
			}

			graphicsAPI.BindMesh(command.mesh); //Binds the mesh
			graphicsAPI.DrawMesh(command.mesh);
		}

		commandList.clear(); //Clears command list as commands have been completed (will be added every update).
	}
}