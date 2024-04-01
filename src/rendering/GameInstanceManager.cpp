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
		m_spriteInstanceBuilder->setSharedBufferAllocator(m_bufferAllocator);
		m_spriteInstanceBuilder->setSharedTextureLoader(m_sharedTextureLoader);
	}

	void GameInstanceManager::unloadCurrentScene()
	{
		// Time to free any previously loaded scene objects:
		// TODO: Also free allocated vertex/element buffers

		m_instances.clear();
		m_bufferAllocator->Free(m_bufferAllocator->GetAllocatedObjectCount());
		m_renderObjectAllocator->Free();
	}

	EngineStatus GameInstanceManager::loadScene(const GameSceneConfig& sceneConfig, const std::shared_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		const size_t maxObjects = static_cast<size_t>(sceneConfig.getMaxInstanceCount());

		m_renderObjectAllocator->Allocate(maxObjects);
		m_renderObjectAllocator->Expand(maxObjects);

		// TODO: Don't keep allocating new memory. Reuse exisitng heap memory!
		m_bufferAllocator->Reserve(maxObjects << 1);
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

		EngineStatus allocateStatus = { };
		for (const auto& instanceConfig : sceneConfig.getInstanceConfigs())
		{
			switch (instanceConfig->getType())
			{
				case GameInstanceType::SPRITE: {
					RenderObject* renderObject = m_renderObjectAllocator->Get();

					std::vector<std::unique_ptr<IRenderComponent>> renderComponents = m_spriteInstanceBuilder->getRenderComponents(instanceConfig.get());
					for (const auto& renderComponent : renderComponents)
					{
						renderComponent->onRenderObjectAllocated(renderObject);
					}

					allocateStatus = renderingSubsystem->registerRenderObject("sprite", renderComponents);

					if (!allocateStatus)
					{
						break;
					}

					std::unique_ptr<GameInstance> gameInstance = std::make_unique<GameInstance>();
					gameInstance->setRenderObject(renderObject);
					gameInstance->setRenderComponents(std::move(renderComponents));
					
					m_instances.push_back(std::move(gameInstance));

					break;
				}
				default: {
					allocateStatus = { "Failed to allocate game instance. Unknown instance type", true };
					break;
				}
			}
		}

		return allocateStatus;
	}
}