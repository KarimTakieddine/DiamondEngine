#include <stdexcept>

#include "RenderingSubsystem.h"

namespace diamond_engine
{
	RenderingSubsystem::RenderingSubsystem(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) : m_sharedShaderStore(sharedShaderStore)
	{
		if (!sharedShaderStore)
		{
			throw std::runtime_error("Failed to instantiate rendering subsystem. No shared shader store was set");
		}

		m_vertexArrayAllocator = std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays);
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

		if (m_activeRenderers.find(shaderProgramName) != m_activeRenderers.cend())
		{
			return { "Failed to register renderer. Already active: " + shaderProgramName, true };
		}

		m_activeRenderers.insert(
			{ shaderProgramName, std::make_unique<Renderer>(shaderProgram) } );

		return { };
	}

	EngineStatus RenderingSubsystem::registerRenderObject(const std::string& shaderProgramName, const std::vector<RenderUpload>& uploads)
	{
		Renderer* renderer = getRenderer(shaderProgramName);

		if (!renderer)
		{
			return { "Failed to register RenderObject. Active renderer not found: " + shaderProgramName, true };
		}

		renderer->registerUploadObject(uploads);

		RenderDrawCall& renderDrawCall = renderer->getInstructions().back().drawCall;

		renderDrawCall.vertexArrayObject	= m_vertexArrayAllocator->Get();
		renderDrawCall.drawMode				= m_drawMode;

		return { };
	}

	Renderer* RenderingSubsystem::getRenderer(const std::string& shaderProgramName) const
	{
		auto rendererIt = m_activeRenderers.find(shaderProgramName);

		if (rendererIt == m_activeRenderers.cend())
		{
			return nullptr;
		}

		return rendererIt->second.get();
	}
}