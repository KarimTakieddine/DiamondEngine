#pragma once

#include "Renderer.h"
#include "GLAllocator.h"

namespace diamond_engine
{
	class Camera;
	class IRenderComponent;
	class RenderingSubsystem
	{
	public:
		RenderingSubsystem();

		void setMaxRendererCount(GLsizei maxInstanceCount);
		void freeAllocatedInstances();

		// TODO: The reverse of this i.e. what happens when we want to get rid of one?
		EngineStatus registerRenderer(const std::string& shaderProgramName);
		EngineStatus registerRenderObject(const std::string& shaderProgramName, const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents) const;

		Renderer* getRenderer(const std::string& shaderProgramName) const;
		void render(const std::string& shaderProgramName) const;
		void renderAll() const;

		const std::shared_ptr<Camera>& getCamera() const;

	private:
		std::unordered_map<std::string, std::unique_ptr<Renderer>> m_renderers;
		std::vector<std::string> m_registeredRenderers;
		std::shared_ptr<Camera> m_camera{ nullptr };
		std::unique_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
	};
}
