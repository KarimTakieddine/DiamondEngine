#pragma once

#include <memory>

#include "AlignedAllocator.hpp"
#include "EngineStatus.h"
#include "RenderObject.h"
#include "GameSceneConfig.h"
#include "SpriteInstanceManager.h"

namespace diamond_engine
{
	class RenderingSubsystem;
	class SharedShaderStore;
	class TextureLoader;
	class Camera;
	class GameInstanceManager
	{
	public:
		GameInstanceManager(
			const std::shared_ptr<SharedShaderStore>& sharedShaderStore,
			const std::shared_ptr<TextureLoader>& sharedTextureLoader);

		void unloadCurrentScene();

		EngineStatus loadScene(const GameSceneConfig& sceneConfig);

	private:
		std::shared_ptr<SharedShaderStore> m_sharedShaderStore;
		std::shared_ptr<TextureLoader> m_sharedTextureLoader;
		std::shared_ptr<AlignedAllocator<RenderObject, 4>> m_renderObjectAllocator{ nullptr };
		std::shared_ptr<RenderingSubsystem> m_renderingSubsystem{ nullptr };
		std::shared_ptr<Camera> m_camera{ nullptr };

		std::unique_ptr<SpriteInstanceManager> m_spriteInstanceManager{ nullptr };
	};
}
