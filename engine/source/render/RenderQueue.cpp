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

	void RenderQueue::Draw(GraphicsAPI& graphicsAPI)
	{
		for (auto& command : commandList)
		{
			graphicsAPI.BindMaterial(command.material); //Binds material
			command.material->GetShaderProgram()->SetUniform("uModel", command.modelMatrix);
			graphicsAPI.BindMesh(command.mesh); //Binds the mesh
			graphicsAPI.DrawMesh(command.mesh);
		}

		commandList.clear(); //Clears command list as commands have been completed (will be added every update).
	}
}