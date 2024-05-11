#include "Camera.h"
#include "IRenderComponent.h"
#include "RenderingSubsystem.h"
#include "SharedShaderStore.h"

namespace diamond_engine
{
	RenderingSubsystem::RenderingSubsystem() : m_camera(std::make_unique<Camera>()), m_uniformBufferAgent(std::make_unique<UniformBufferAgent>()) // TODO: Share this?
	{
		m_uniformBufferAgent->reserveCapacity(256);

		m_vertexArrayAllocator = std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays);
		m_vertexArrayAllocator->Reserve(256);

		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);
	}

	void RenderingSubsystem::setMaxRendererCount(GLsizei maxRendererCount)
	{
		m_uniformBufferAgent->allocateBuffers(maxRendererCount);
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

	EngineStatus RenderingSubsystem::registerRenderer(MeshType meshType, GLenum drawType, GLenum drawMode, const std::vector<VertexAttribute>& vertexAttributes, const std::string& shaderProgramName)
	{
		const auto shaderProgram = SharedShaderStore::getInstance()->FindProgram(shaderProgramName);

		if (!shaderProgram)
		{
			return { "Failed to register renderer. Shader program not found: " + shaderProgramName, true };
		}

		if (m_renderers.find(shaderProgramName) != m_renderers.cend())
		{
			return { "Failed to register renderer. Already active: " + shaderProgramName, true };
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
			{ shaderProgramName, std::move(renderer) });

		m_registeredRenderers.push_back(shaderProgramName);

		return { };
	}

	EngineStatus RenderingSubsystem::registerRenderObject(const std::string& shaderProgramName, const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents) const
	{
		Renderer* renderer = getRenderer(shaderProgramName);

		if (!renderer)
		{
			return { "Failed to register RenderObject. Active renderer not found: " + shaderProgramName, true };
		}

		RenderDrawCall renderDrawCall{ };
		renderer->registerRenderInstruction(renderComponents, &renderDrawCall);

		return { };
	}

	Renderer* RenderingSubsystem::getRenderer(const std::string& shaderProgramName) const
	{
		auto rendererIt = m_renderers.find(shaderProgramName);

		if (rendererIt == m_renderers.cend())
		{
			return nullptr;
		}

		return rendererIt->second.get();
	}

	void RenderingSubsystem::renderAll() const
	{
		m_uniformBufferAgent->uploadBufferData(m_cameraUniformBuffer);

		for (size_t i = 0; i < m_registeredRenderers.size(); ++i)
		{
			Renderer* registeredRenderer = getRenderer(m_registeredRenderers[i]);

			if (!registeredRenderer)
			{
				throw std::runtime_error("Attempt to access invalid renderer. Name: " + m_registeredRenderers[i]);
			}

			registeredRenderer->render();
		}
	}

	const std::shared_ptr<Camera>& RenderingSubsystem::getCamera() const
	{
		return m_camera;
	}
}