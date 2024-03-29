#pragma once

#include "Renderer.h"
#include "SharedShaderStore.h"
#include "GLAllocator.h"

namespace diamond_engine
{
	class RenderingSubsystem
	{
	public:
		explicit RenderingSubsystem(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

		void setMaxInstanceCount(GLsizei maxInstanceCount);
		void freeAllocatedInstances();

		EngineStatus registerRenderer(const std::string& shaderProgramName);
		EngineStatus registerRenderObject(const std::string& shaderProgramName, const std::vector<RenderUpload>& uploads);

		Renderer* getRenderer(const std::string& shaderProgramName) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<Renderer>> m_activeRenderers;
		std::shared_ptr<SharedShaderStore> m_sharedShaderStore{ nullptr };
		std::unique_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
		GLenum m_drawMode{ GL_TRIANGLES };
	};
}
