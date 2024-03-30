#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace diamond_engine
{
	struct EngineStatus;
	class GraphicsContext;
	class TextureLoader;
	class SharedShaderStore;
	class GameInstanceManager;
	class GameSceneConfig;
	class GameEngine
	{
	public:
		GameEngine(
			const std::shared_ptr<GraphicsContext>& graphicsContext,
			const std::shared_ptr<TextureLoader>& sharedTextureLoader,
			const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

		void addScene(const std::string& file, const std::string& name);
		void addScene(const std::string& name, std::unique_ptr<GameSceneConfig> sceneConfig);
		void removeScene(const std::string& name);
		void loadScene(const std::string& name);

	private:
		std::unordered_map<std::string, std::unique_ptr<GameSceneConfig>> m_gameScenes;
		std::unique_ptr<GameInstanceManager> m_instanceManager{ nullptr };
		std::string m_currentScene;
	};
}
