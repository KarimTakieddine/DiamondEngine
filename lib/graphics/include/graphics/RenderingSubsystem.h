#pragma once

#include "GLAllocator.h"
#include "MemoryPoolAllocator.hpp"
#include "Renderer.h"
#include "UniformBufferAgent.h"
#include "VertexAttribute.h"

namespace diamond_engine
{
	class Camera;
	class GameInstance;
	class IRenderComponent;
	class RenderingSubsystem
	{
	public:
		RenderingSubsystem();

		void setMaxRendererCount(GLsizei maxInstanceCount);
		void freeAllocatedRenderers();

		// TODO: The reverse of this i.e. what happens when we want to get rid of one?
		EngineStatus registerRenderer(MeshType meshType, GLenum drawMode, const std::string& name, const std::string& shaderProgramName);
		EngineStatus registerRenderObject(const std::string& name, const RenderComponentList& renderComponents) const;
		EngineStatus unregisterRenderObject(const std::string& name, const RenderComponentList& renderComponents) const;
		EngineStatus allocateVertexState(const std::string& name, GLenum drawType, const std::vector<VertexAttribute>& vertexAttributes);
		EngineStatus releaseVertexState();

		Renderer* getRenderer(const std::string& name) const;
		void preRender();
		void render(const std::string& name, const std::vector<std::unique_ptr<GameInstance>>& gameInstances) const;

		const std::shared_ptr<Camera>& getCamera() const;

		void setBackgroundColor(const glm::vec4& backgroundColor);

	private:
		std::unordered_map<std::string, std::unique_ptr<Renderer>> m_renderers;
		UniformBuffer m_cameraUniformBuffer;
		std::shared_ptr<Camera> m_camera{ nullptr };
		std::unique_ptr<UniformBufferAgent> m_uniformBufferAgent{ nullptr };
		std::unique_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
		std::unique_ptr<GraphicsMemoryPool> m_graphicsMemoryPool{nullptr};
		glm::vec4 m_backgroundColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
