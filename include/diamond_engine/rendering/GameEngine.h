#pragma once

#include <memory>

#include "SceneConfig.h"

namespace diamond_engine
{
	class GraphicsContext;
	class TextureLoader;
	class SharedShaderStore;
	class GameInstanceManager;
	class GameEngine
	{
	public:
		GameEngine(
			const std::shared_ptr<GraphicsContext>& graphicsContext,
			const std::shared_ptr<TextureLoader>& sharedTextureLoader,
			const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

		void loadScene(const SceneConfig& sceneConfig);

	private:
		std::unique_ptr<GameInstanceManager> m_instanceManager{ nullptr };
	};
}
