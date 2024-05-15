#include "Camera.h"
#include "IRenderComponent.h"
#include "RenderingSubsystem.h"
#include "SharedShaderStore.h"

namespace diamond_engine
{
	RenderingSubsystem::RenderingSubsystem() :
		m_camera(std::make_unique<Camera>()),
		m_uniformBufferAgent(std::make_unique<UniformBufferAgent>()),
		m_graphicsMemoryPool(std::make_unique< GraphicsMemoryPool>())
	{
		m_graphicsMemoryPool->allocate(4096);

		m_uniformBufferAgent->reserveCapacity(1);
		m_uniformBufferAgent->allocateBuffers(1); // Camera buffer. Common to all renderers / shader programs

		m_vertexArrayAllocator = std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays);
		m_vertexArrayAllocator->Reserve(256);

		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);
	}

	void RenderingSubsystem::setMaxRendererCount(GLsizei maxRendererCount)
	{
		m_vertexArrayAllocator->Allocate(maxRendererCount);

		m_cameraUniformBuffer			= m_uniformBufferAgent->allocateBuffer(0, GL_DYNAMIC_DRAW);
		m_cameraUniformBuffer.segments	= {
			{
				&m_camera->GetProjection(),
				sizeof(glm::mat4),
				0
			},
			{
				&m_camera->GetTransform().getLocalToWorld(),
				sizeof(glm::mat4),
				sizeof(glm::mat4)
			},
			{
				&m_camera->GetTransform().getLocalRotation(),
				sizeof(glm::mat4),
				2 * sizeof(glm::mat4)
			},
			{
				&m_camera->GetView(),
				sizeof(glm::mat4),
				3 * sizeof(glm::mat4)
			}
		};
		m_uniformBufferAgent->registerUniformBuffer(m_cameraUniformBuffer);
	}

	void RenderingSubsystem::freeRegisteredInstructions()
	{
		for (const auto& registeredRenderer : m_registeredRenderers)
		{
			getRenderer(registeredRenderer)->clearRenderInstructions();
		}
	}

	void RenderingSubsystem::freeAllocatedRenderers()
	{
		m_vertexArrayAllocator->Free(m_vertexArrayAllocator->GetAllocatedObjectCount());
		m_uniformBufferAgent->freeAllBuffers();
	}

	EngineStatus RenderingSubsystem::registerRenderer(MeshType meshType, GLenum drawType, GLenum drawMode, const std::vector<VertexAttribute>& vertexAttributes, const std::string& name, const std::string& shaderProgramName)
	{
		const auto shaderProgram = SharedShaderStore::getInstance()->FindProgram(shaderProgramName);

		if (!shaderProgram)
		{
			return { "Failed to register renderer. Shader program not found: " + shaderProgramName, true };
		}

		if (m_renderers.find(name) != m_renderers.cend())
		{
			return { "Failed to register renderer. Already active: " + name, true };
		}

		EngineStatus uniformBufferStatus = m_uniformBufferAgent->bindUniformBuffer(
			m_cameraUniformBuffer,
			shaderProgram,
			"CameraMatrices",
			{ "cameraProjection", "cameraLocalToWorld", "cameraLocalRotation", "cameraView" });

		if (!uniformBufferStatus)
		{
			return uniformBufferStatus;
		}

		std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(m_vertexArrayAllocator->Get(), meshType, drawMode, shaderProgram);
		renderer->uploadMeshData(vertexAttributes, drawType);

		m_renderers.insert(
			{ name, std::move(renderer) });

		m_registeredRenderers.push_back(name);

		return { };
	}

	EngineStatus RenderingSubsystem::registerRenderObject(const std::string& name, const RenderComponentList& renderComponents) const
	{
		Renderer* renderer = getRenderer(name);

		if (!renderer)
		{
			return { "Failed to register RenderObject. Active renderer not found: " + name, true };
		}

		renderer->allocateGraphicsMemory(renderComponents, m_graphicsMemoryPool);
		renderer->bindToShaderProgram(renderComponents);

		RenderDrawCall renderDrawCall{ };
		renderer->registerRenderInstruction(renderComponents, &renderDrawCall);

		return { };
	}

	EngineStatus RenderingSubsystem::unregisterRenderObject(const std::string& name, const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents) const
	{
		Renderer* renderer = getRenderer(name);

		if (!renderer)
		{
			return { "Failed to unregister RenderObject. Active renderer not found: " + name, true };
		}

		renderer->releaseGraphicsMemory(renderComponents, m_graphicsMemoryPool);

		return { };
	}

	Renderer* RenderingSubsystem::getRenderer(const std::string& name) const
	{
		auto rendererIt = m_renderers.find(name);

		if (rendererIt == m_renderers.cend())
		{
			return nullptr;
		}

		return rendererIt->second.get();
	}

	void RenderingSubsystem::preRender()
	{
		glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_uniformBufferAgent->uploadBufferData(m_cameraUniformBuffer);
	}

	void RenderingSubsystem::render(const std::string& name, const RenderComponentList& renderComponents) const
	{
		Renderer* registeredRenderer = getRenderer(name);

		if (!registeredRenderer)
		{
			throw std::runtime_error("Attempt to access invalid renderer. Name: " + name);
		}

		registeredRenderer->render(renderComponents, m_graphicsMemoryPool);
	}

	void RenderingSubsystem::renderAll() const
	{
		for (size_t i = 0; i < m_registeredRenderers.size(); ++i)
		{
			Renderer* registeredRenderer = getRenderer(m_registeredRenderers[i]);

			if (!registeredRenderer)
			{
				throw std::runtime_error("Attempt to access invalid renderer. Name: " + m_registeredRenderers[i]);
			}

			// registeredRenderer->render();
		}
	}

	const std::shared_ptr<Camera>& RenderingSubsystem::getCamera() const
	{
		return m_camera;
	}

	void RenderingSubsystem::setBackgroundColor(const glm::vec4& backgroundColor)
	{
		m_backgroundColor = backgroundColor;
	}
}