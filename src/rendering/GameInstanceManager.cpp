#include "GameInstanceManager.h"
#include "GLAllocator.h"
#include "RenderingSubsystem.h"

namespace diamond_engine
{
	GameInstanceManager::GameInstanceManager(const std::shared_ptr<TextureLoader>& sharedTextureLoader) :
		m_sharedTextureLoader(sharedTextureLoader),
		m_renderObjectAllocator(std::make_unique<AlignedAllocator<RenderObject, 4>>()),
		m_bufferAllocator(std::make_shared<GLAllocator>(glGenBuffers, glDeleteBuffers)),
		m_spriteInstanceBuilder(std::make_unique<SpriteInstanceBuilder>())
	{
		m_renderObjectAllocator->Allocate(256);
		m_bufferAllocator->Reserve(512);
		m_spriteInstanceBuilder->setSharedBufferAllocator(m_bufferAllocator);
		m_spriteInstanceBuilder->setSharedTextureLoader(m_sharedTextureLoader);
	}

	void GameInstanceManager::setRenderingSubsystem(const std::shared_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		m_renderingSubsystem = renderingSubsystem;
	}

	void GameInstanceManager::unloadCurrentScene()
	{
		// Time to free any previously loaded scene objects:
		// TODO: Also free allocated vertex/element buffers

		m_instances.clear();
		m_bufferAllocator->Free(m_bufferAllocator->GetAllocatedObjectCount());
		m_renderObjectAllocator->Free(m_renderObjectAllocator->GetAllocatedObjectCount());
	}

	std::unique_ptr<GameInstance> GameInstanceManager::createInstance()
	{
		std::unique_ptr<GameInstance> result = std::make_unique<GameInstance>();
		result->setRenderObject(m_renderObjectAllocator->Get());
		return result;
	}

	EngineStatus GameInstanceManager::registerInstance(const GameInstanceConfig* instanceConfig, std::unique_ptr<GameInstance> instance)
	{
		if (!instance)
		{
			return { "Failed to register game instance. No allocated instance was provided", true };
		}

		const GameInstanceType instanceType = instanceConfig->getType();

		std::vector<std::unique_ptr<IRenderComponent>> renderComponents;
		switch (instanceType)
		{
		case GameInstanceType::SPRITE:
			renderComponents = m_spriteInstanceBuilder->getRenderComponents(instanceConfig);
			break;
		default:
			return { "Failed to register game instance of unknown type", true };
		}

		for (auto& renderComponent : renderComponents)
		{
			EngineStatus status = renderComponent->onRenderObjectAllocated(instance->getRenderObject());

			if (!status)
			{
				return status;
			}

			instance->acquireRenderComponent(std::move(renderComponent));
		}

		EngineStatus registerStatus = registerInstance(instanceType, instance.get());

		if (!registerStatus)
		{
			return registerStatus;
		}

		m_instances.push_back(std::move(instance));

		return { };
	}

	EngineStatus GameInstanceManager::registerInstance(GameInstanceType instanceType, const GameInstance* instance)
	{
		EngineStatus registerStatus;

		switch (instanceType)
		{
		case GameInstanceType::SPRITE:
			registerStatus = m_renderingSubsystem->registerRenderObject("sprite_2", instance->getRenderComponents());
			break;
		default:
			registerStatus = { "Failed to register game instance of unknown type", true };
			break;
		}

		return registerStatus;
	}

	EngineStatus GameInstanceManager::loadScene(const GameSceneConfig& sceneConfig)
	{
		const size_t maxObjects = static_cast<size_t>(sceneConfig.getMaxInstanceCount());

		m_renderObjectAllocator->Expand(maxObjects);
		m_bufferAllocator->Allocate(maxObjects << 1);

		/*
		* Here we need to allocate the render object
		* Build the render components
		* Pass them to the render instance manager
		* Then comes out a draw call
		* We call onDrawCallRegistered for all components
		* Assign them to a game instance
		* Voila! -- DONE YO!
		*/

		for (const auto& instanceConfig : sceneConfig.getInstanceConfigs())
		{
			auto gameInstance = createInstance();

			if (!gameInstance)
			{
				return { "Failed to create game instance", true };
			}

			EngineStatus registerStatus = registerInstance(instanceConfig.get(), std::move(gameInstance));

			if (!registerStatus)
			{
				return registerStatus;
			}
		}

		return { };
	}
}