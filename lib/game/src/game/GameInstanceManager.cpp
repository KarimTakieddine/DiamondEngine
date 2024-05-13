#include "Collider2DComponent.h"
#include "GameInstanceBuilder.h"
#include "GameInstanceManager.h"
#include "MaterialRenderComponent.h"
#include "TransformRenderComponent.h"
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
		m_colliders.clear();
		m_instances.clear();
		m_renderObjectAllocator->Free(m_renderObjectAllocator->GetAssignedObjectCount());
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
		case GameInstanceType::SPRITE: {
			registerStatus = m_renderingSubsystem->registerRenderObject("sprite_renderer", instance->getRenderComponents());

			auto collider2DBehaviour = instance->extractBehaviourComponent("Collider2D");
			if (collider2DBehaviour)
			{
				auto collider2DInstance = createInstance();

				dynamic_cast<Collider2DComponent*>(collider2DBehaviour.get())->setTarget(instance->getRenderObject());
				collider2DInstance->acquireBehaviourComponent(std::move(collider2DBehaviour));

				std::unique_ptr<MaterialRenderComponent> colliderMaterial = std::make_unique<MaterialRenderComponent>();
				colliderMaterial->setColor({ 0.0f, 1.0f, 0.0f });
				collider2DInstance->acquireRenderComponent(std::move(colliderMaterial));

				std::unique_ptr<TransformRenderComponent> colliderTransform = std::make_unique<TransformRenderComponent>();
				collider2DInstance->acquireRenderComponent(std::move(colliderTransform));

				registerStatus = m_renderingSubsystem->registerRenderObject("collider_2d_renderer", collider2DInstance->getRenderComponents());

				if (!registerStatus)
				{
					return registerStatus;
				}

				m_colliders.push_back(std::move(collider2DInstance));
			}

			break;
		}
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
		const size_t gameInstanceCount = static_cast<size_t>(sceneConfig.getInstanceConfigs().size());

		m_renderObjectAllocator->Expand(gameInstanceCount << 1);

		m_renderingSubsystem->setBackgroundColor(sceneConfig.getBackgroundColor());

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

	void GameInstanceManager::updateInstances(GLfloat deltaTime)
	{
		for (const auto& instance : m_instances)
		{
			for (const auto& behaviourComponent : instance->getBehaviourComponents())
			{
				behaviourComponent->update(deltaTime);
			}
		}

		for (const auto& colliderInstance : m_colliders)
		{
			for (const auto& behaviourComponent : colliderInstance->getBehaviourComponents())
			{
				behaviourComponent->update(deltaTime);
			}
		}
	}
}