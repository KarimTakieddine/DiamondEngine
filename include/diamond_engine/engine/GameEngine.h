#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "EngineConfig.h"
#include "GraphicsContext.h"
#include "KeyboardConfig.h"
#include "GameInstanceManager.h"
#include "RenderingSubsystem.h"
#include "CollisionResolver2D.h"

namespace diamond_engine
{
	struct EngineStatus;
	class GraphicsContext;
	class GameSceneConfig;
	class GameEngine
	{
	public:
		GameEngine();

		void initialize(const EngineConfig& engineConfig);
		void run();

		void addScene(const std::string& name, const std::string& file);
		void addScene(const std::string& name, std::unique_ptr<GameSceneConfig> sceneConfig);
		void removeScene(const std::string& name);
		void loadScene(const std::string& name);
		void unloadCurrentScene();

	private:
		void initializeInput(const KeyboardConfig& keyboardConfig, const ControllerConfig& controllerConfig);
		void onWindowUpdate(GLfloat deltaTime);
		void onWindowResize(const Size& size);
		std::unique_ptr<GameInstance> buildGameInstance(const GameInstanceConfig* config);

		std::unordered_map<std::string, std::unique_ptr<GameSceneConfig>> m_gameScenes;
		std::vector<std::unique_ptr<GameInstance>> m_spriteInstances;
		std::vector<std::unique_ptr<GameInstance>> m_collider2DInstances;
		std::unique_ptr<GraphicsContext> m_graphicsContext{ nullptr };
		std::shared_ptr<RenderingSubsystem> m_renderingSubsystem{ nullptr };
		std::unique_ptr<GameInstanceManager> m_instanceManager{ nullptr };
		std::unique_ptr<CollisionResolver2D> m_collisionResolver2D{ nullptr };
		std::string m_currentScene;
	};
}
