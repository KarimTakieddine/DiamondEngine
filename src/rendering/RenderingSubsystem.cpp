#include <stdexcept>

#include "Camera.h"
#include "IRenderComponent.h"
#include "RenderingSubsystem.h"

namespace diamond_engine
{
	RenderingSubsystem::RenderingSubsystem(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) : m_sharedShaderStore(sharedShaderStore), m_camera(std::make_unique<Camera>()) // TODO: Share this?
	{
		if (!sharedShaderStore)
		{
			throw std::runtime_error("Failed to instantiate rendering subsystem. No shared shader store was set");
		}

		m_vertexArrayAllocator = std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays);

		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);
	}

	void RenderingSubsystem::setMaxInstanceCount(GLsizei maxInstanceCount)
	{
		m_vertexArrayAllocator->Reserve(maxInstanceCount);
		m_vertexArrayAllocator->Allocate(maxInstanceCount);
	}

	void RenderingSubsystem::freeAllocatedInstances()
	{
		m_vertexArrayAllocator->Free(m_vertexArrayAllocator->GetAllocatedObjectCount());
	}

	EngineStatus RenderingSubsystem::registerRenderer(const std::string& shaderProgramName)
	{
		const auto& shaderProgram = m_sharedShaderStore->FindProgram(shaderProgramName);

		if (!shaderProgram)
		{
			return { "Failed to register renderer. Shader program not found: " + shaderProgramName, true };
		}

		if (m_renderers.find(shaderProgramName) != m_renderers.cend())
		{
			return { "Failed to register renderer. Already active: " + shaderProgramName, true };
		}

		std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(shaderProgram);
		renderer->setCamera(m_camera);
		m_renderers.insert(
			{ shaderProgramName, std::move(renderer) });

		m_registeredRenderers.push_back(shaderProgramName);

		return { };
	}

	EngineStatus RenderingSubsystem::registerRenderObject(const std::string& shaderProgramName, const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents)
	{
		Renderer* renderer = getRenderer(shaderProgramName);

		if (!renderer)
		{
			return { "Failed to register RenderObject. Active renderer not found: " + shaderProgramName, true };
		}

		RenderDrawCall renderDrawCall{ m_vertexArrayAllocator->Get() };
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

	void RenderingSubsystem::render(const std::string& shaderProgramName)
	{
		Renderer* registeredRenderer = getRenderer(shaderProgramName);

		if (!registeredRenderer)
		{
			throw std::runtime_error("Attempt to access invalid renderer. Name: " + shaderProgramName);
		}

		registeredRenderer->render();
	}

	void RenderingSubsystem::renderAll()
	{
		for (const auto& registeredRenderer : m_registeredRenderers)
		{
			render(registeredRenderer);
		}
	}
}