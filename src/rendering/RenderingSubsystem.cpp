#include <stdexcept>

#include "Camera.h"
#include "IRenderComponent.h"
#include "RenderingSubsystem.h"
#include "SharedShaderStore.h"

namespace diamond_engine
{
	RenderingSubsystem::RenderingSubsystem() : m_camera(std::make_unique<Camera>()) // TODO: Share this?
	{
		m_vertexArrayAllocator = std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays);
		m_vertexArrayAllocator->Reserve(256);

		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);
	}

	void RenderingSubsystem::setMaxRendererCount(GLsizei maxRendererCount)
	{
		m_vertexArrayAllocator->Allocate(maxRendererCount);
	}

	void RenderingSubsystem::freeAllocatedInstances()
	{
		for (const auto& registeredRenderer : m_registeredRenderers)
		{
			getRenderer(registeredRenderer)->clearRenderInstructions();
		}

		m_vertexArrayAllocator->Free(m_vertexArrayAllocator->GetAllocatedObjectCount());
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

		std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(m_vertexArrayAllocator->Get(), meshType, drawMode, shaderProgram);
		renderer->uploadMeshData(vertexAttributes, drawType);
		renderer->setCamera(m_camera);
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

	void RenderingSubsystem::render(const std::string& shaderProgramName) const
	{
		Renderer* registeredRenderer = getRenderer(shaderProgramName);

		if (!registeredRenderer)
		{
			throw std::runtime_error("Attempt to access invalid renderer. Name: " + shaderProgramName);
		}

		registeredRenderer->render();
	}

	void RenderingSubsystem::renderAll() const
	{
		for (const auto& registeredRenderer : m_registeredRenderers)
		{
			render(registeredRenderer);
		}
	}

	const std::shared_ptr<Camera>& RenderingSubsystem::getCamera() const
	{
		return m_camera;
	}
}