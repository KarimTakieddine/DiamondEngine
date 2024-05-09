#include "GameInstanceBuilder.h"
#include "GameInstanceManager.h"
#include "GLAllocator.h"
#include "RenderingSubsystem.h"

namespace diamond_engine
{
	GameInstanceManager::GameInstanceManager() :
		m_renderObjectAllocator(std::make_unique<AlignedAllocator<RenderObject, 4>>())
	{
		m_renderObjectAllocator->Allocate(256);
	}

	void GameInstanceManager::setRenderingSubsystem(const std::shared_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		m_renderingSubsystem = renderingSubsystem;
	}

	void GameInstanceManager::unloadCurrentScene()
	{
		m_instances.clear();
		m_renderObjectAllocator->Free(m_renderObjectAllocator->GetAllocatedObjectCount());
	}

	std::unique_ptr<GameInstance> GameInstanceManager::createInstance()
	{
		std::unique_ptr<GameInstance> result = std::make_unique<GameInstance>();
		result->setRenderObject(m_renderObjectAllocator->Get());
		return result;
	}

	EngineStatus GameInstanceManager::registerInstance(std::unique_ptr<GameInstance> instance, const GameInstanceConfig* instanceConfig)
	{
		if (!instance)
		{
			return { "Failed to register game instance. No allocated instance was provided", true };
		}

		if (!instanceConfig)
		{
			return { "Failed to register game instance. No instance config was provided", true };
		}

		EngineStatus buildStatus = buildGameInstance(instance, instanceConfig);

		if (!buildStatus)
		{
			return buildStatus;
		}

		EngineStatus registerStatus;
		switch (instanceConfig->getType())
		{
		case GameInstanceType::SPRITE:
			registerStatus = m_renderingSubsystem->registerRenderObject("sprite", instance->getRenderComponents());
			break;
		case GameInstanceType::COLLIDER:
			registerStatus = m_renderingSubsystem->registerRenderObject("unlit_color", instance->getRenderComponents());
			break;
		default:
			registerStatus = { "Failed to register game instance of unknown type", true };
			break;
		}

		if (!registerStatus)
		{
			return registerStatus;
		}

		m_instances.push_back(std::move(instance));

		return { };
	}

	EngineStatus GameInstanceManager::loadScene(const GameSceneConfig& sceneConfig)
	{
		const size_t maxObjects = static_cast<size_t>(sceneConfig.getMaxInstanceCount());

		m_renderObjectAllocator->Expand(maxObjects);

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

			EngineStatus registerStatus = registerInstance(std::move(gameInstance), instanceConfig.get());

			if (!registerStatus)
			{
				return registerStatus;
			}
		}

		return { };
	}
}