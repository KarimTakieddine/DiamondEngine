#include "RenderInstanceManager.h"
#include "RenderingSubsystem.h"

namespace diamond_engine
{
	EngineStatus RenderInstanceManager::setSharedObjectAllocator(const SharedRenderObjectAllocator& sharedObjectAllocator)
	{
		if (!sharedObjectAllocator)
		{
			return { "Invalid SharedObjectAllocator instance set", true };
		}

		m_objectAllocator = sharedObjectAllocator;

		return { };
	}

	EngineStatus RenderInstanceManager::setSharedRenderingSubsystem(const SharedRenderingSubsystem& sharedRenderingSubsystem)
	{
		if (!sharedRenderingSubsystem)
		{
			return { "Invalid SharedRenderingSubsystem instance set", true };
		}

		m_renderingSubsystem = sharedRenderingSubsystem;

		return { };
	}

	EngineStatus RenderInstanceManager::setShaderProgramName(const std::string& shaderProgramName)
	{
		EngineStatus registerStatus = m_renderingSubsystem->registerRenderer(shaderProgramName);

		if (!registerStatus)
		{
			return registerStatus;
		}

		m_shaderProgramName = shaderProgramName;

		m_renderingSubsystem->getRenderer(m_shaderProgramName)->setCamera(m_camera);

		return { };
	}

	EngineStatus RenderInstanceManager::allocateInstance(const GameInstanceConfig* instanceConfig)
	{
		RenderObject* renderObject = m_objectAllocator->Get();

		if (!renderObject)
		{
			return { "Failed to allocate render object", true };
		}

		std::vector<std::unique_ptr<IRenderComponent>> components = getComponents(instanceConfig);

		for (const auto& component : components)
		{
			EngineStatus status = component->onRenderObjectAllocated(renderObject);

			if (!status)
			{
				return status;
			}
		}

		Renderer* renderer = m_renderingSubsystem->getRenderer(m_shaderProgramName);

		if (!renderer)
		{
			return { "Could not find renderer named: " + m_shaderProgramName, true };
		}

		for (const auto& component : components)
		{
			EngineStatus status = component->bindToShaderProgram(renderer->getShaderProgram());

			if (!status)
			{
				return status;
			}
		}

		std::vector<RenderUpload> uploads;
		for (const auto& component : components)
		{
			std::vector<RenderUpload>& componentUploads = component->getUploads();
			uploads.insert(uploads.end(), componentUploads.begin(), componentUploads.end());
		}

		EngineStatus registerStatus = m_renderingSubsystem->registerRenderObject(m_shaderProgramName, uploads);

		if (!registerStatus)
		{
			return registerStatus;
		}

		glUseProgram(renderer->getShaderProgram()->GetObject());
		RenderDrawCall& renderDrawCall = renderer->getInstructions().back().drawCall;
		for (const auto& component : components)
		{
			component->onDrawCallRegistered(&renderDrawCall);
		}

		std::unique_ptr<RenderInstance> instance = std::make_unique<RenderInstance>();
		instance->setObject(renderObject);
		instance->setComponents(std::move(components));
		m_instances.push_back(std::move(instance));

		return { };
	}

	void RenderInstanceManager::setCamera(const std::shared_ptr<Camera>& camera)
	{
		m_renderingSubsystem->getRenderer(m_shaderProgramName)->setCamera(camera);

		m_camera = camera;
	}
}