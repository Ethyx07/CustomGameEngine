#include "render/RenderQueue.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "graphics/GraphicsAPI.h"
#include "graphics/ShaderProgram.h"

namespace eng
{
	void RenderQueue::Init()
	{
		mesh2D = Mesh::CreatePlane();
	}

	void RenderQueue::Submit(const RenderCommand& command)
	{
		commandList.push_back(command);  //Adds render command to list. command contains a mesh and material
	}

	void RenderQueue::Submit(const RenderCommand2D& command)
	{
		commandList2D.push_back(command); //Adds 2d command to list
	}

	void RenderQueue::Draw(GraphicsAPI& graphicsAPI, const CameraData& cameraData, const std::vector<LightData> lights)
	{
		//3D rendering
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
				shaderProgram->SetUniform("uLight.direction", glm::normalize(-light.position));
			}

			graphicsAPI.BindMesh(command.mesh); //Binds the mesh
			graphicsAPI.DrawMesh(command.mesh);
			graphicsAPI.UnbindMesh(command.mesh); //Unbinds meshes VAO. Cleans up rendering process to reduce memory usage
		}

		commandList.clear(); //Clears command list as commands have been completed (will be added every update).

		//2D rendering
		graphicsAPI.SetDepthTestEnabled(false);
		graphicsAPI.SetBlendMode(BlendMode::Alpha);
		const auto shaderProgram2D = graphicsAPI.GetDefault2DShaderProgram();
		shaderProgram2D->Bind();
		mesh2D->Bind();
		for (auto& command : commandList2D)
		{
			//Rendering Sprites
			shaderProgram2D->SetUniform("uModel", command.modelMatrix);
			shaderProgram2D->SetUniform("uView", cameraData.viewMatrix);
			shaderProgram2D->SetUniform("uProjection", cameraData.orthographicMatrix);
			shaderProgram2D->SetUniform("uSize", command.size.x, command.size.y);
			shaderProgram2D->SetUniform("uPivot", command.pivot.x, command.pivot.y);
			shaderProgram2D->SetUniform("uUVMin", command.lowerLeftUV.x, command.lowerLeftUV.y);
			shaderProgram2D->SetUniform("uUVMax", command.upperRightUV.x, command.upperRightUV.y);
			shaderProgram2D->SetUniform("uColour", command.colour);
			shaderProgram2D->SetTexture("uTex", command.texture);

			mesh2D->Draw();
		}
		mesh2D->Unbind();
		graphicsAPI.SetBlendMode(BlendMode::Disabled);
		graphicsAPI.SetDepthTestEnabled(true);
	}
}