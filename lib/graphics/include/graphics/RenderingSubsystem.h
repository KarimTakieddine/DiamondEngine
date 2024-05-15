#pragma once

#include "Renderer.h"
#include "GLAllocator.h"
#include "VertexAttribute.h"
#include "UniformBufferAgent.h"

#include "MemoryPoolAllocator.hpp"

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
		void freeRegisteredInstructions();

		// TODO: The reverse of this i.e. what happens when we want to get rid of one?
		EngineStatus registerRenderer(MeshType meshType, GLenum drawType, GLenum drawMode, const std::vector<VertexAttribute>& vertexAttributes, const std::string& name, const std::string& shaderProgramName);
		EngineStatus registerRenderObject(const std::string& name, const RenderComponentList& renderComponents) const;
		EngineStatus unregisterRenderObject(const std::string& name, const RenderComponentList& renderComponents) const;

		Renderer* getRenderer(const std::string& name) const;
		void preRender();
		void render(const std::string& name, const std::vector<std::unique_ptr<GameInstance>>& gameInstances) const;
		void renderAll() const;

		const std::shared_ptr<Camera>& getCamera() const;

		void setBackgroundColor(const glm::vec4& backgroundColor);
		GLuint getVertexArrayObject(const std::string& name) const;

		// TODO: void render(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<Renderer>> m_renderers;
		std::vector<std::string> m_registeredRenderers;
		UniformBuffer m_cameraUniformBuffer;
		std::shared_ptr<Camera> m_camera{ nullptr };
		std::unique_ptr<UniformBufferAgent> m_uniformBufferAgent{ nullptr };
		std::unique_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
		std::unique_ptr<GraphicsMemoryPool> m_graphicsMemoryPool{nullptr};
		glm::vec4 m_backgroundColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
