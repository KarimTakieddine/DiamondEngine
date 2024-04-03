#pragma once

#include <memory>

#include "AlignedAllocator.hpp"
#include "EngineStatus.h"
#include "GameInstance.h"
#include "GameSceneConfig.h"
#include "RenderObject.h"
#include "SpriteInstanceBuilder.h"

namespace diamond_engine
{
	class GLAllocator;
	class RenderingSubsystem;
	class TextureLoader;
	class GameInstanceManager
	{
	public:
		GameInstanceManager(const std::shared_ptr<TextureLoader>& sharedTextureLoader);

		void setRenderingSubsystem(const std::shared_ptr<RenderingSubsystem>& renderingSubsystem);
		void unloadCurrentScene();
		EngineStatus loadScene(const GameSceneConfig& sceneConfig);
		std::unique_ptr<GameInstance> createInstance();
		EngineStatus registerInstance(const GameInstanceConfig* instanceConfig, std::unique_ptr<GameInstance> instance);
		/* EngineStatus registerInstance(GameInstanceType instanceType, const GameInstance* instance); */

	private:
		std::vector<std::unique_ptr<GameInstance>> m_instances;
		std::shared_ptr<TextureLoader> m_sharedTextureLoader;
		std::shared_ptr<RenderingSubsystem> m_renderingSubsystem{ nullptr };
		std::unique_ptr<AlignedAllocator<RenderObject, 4>> m_renderObjectAllocator{ nullptr };
		std::shared_ptr<GLAllocator> m_bufferAllocator{ nullptr };
		

		std::unique_ptr<SpriteInstanceBuilder> m_spriteInstanceBuilder{ nullptr };
	};
}
