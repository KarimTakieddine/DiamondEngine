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
	class SharedShaderStore;
	class TextureLoader;
	class GameInstanceManager
	{
	public:
		GameInstanceManager(const std::shared_ptr<TextureLoader>& sharedTextureLoader);

		void unloadCurrentScene();

		EngineStatus loadScene(const GameSceneConfig& sceneConfig, const std::shared_ptr<RenderingSubsystem>& renderingSubsystem);

	private:
		std::vector<std::unique_ptr<GameInstance>> m_instances;
		std::shared_ptr<SharedShaderStore> m_sharedShaderStore;
		std::shared_ptr<TextureLoader> m_sharedTextureLoader;
		std::unique_ptr<AlignedAllocator<RenderObject, 4>> m_renderObjectAllocator{ nullptr };
		std::shared_ptr<GLAllocator> m_bufferAllocator{ nullptr };
		

		std::unique_ptr<SpriteInstanceBuilder> m_spriteInstanceBuilder{ nullptr };
	};
}
