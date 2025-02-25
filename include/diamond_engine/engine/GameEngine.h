#pragma once

#include <memory>
#include <string>

#include "EngineConfig.h"
#include "EngineStorage.h"
#include "GraphicsContext.h"
#include "KeyboardConfig.h"
#include "GameInstanceManager.h"
#include "RenderingSubsystem.h"
#include "CollisionSolver2D.h"
#include "TextWindow.h"

namespace diamond_engine
{
	struct EngineStatus;
	class FontLibrary;
	class GraphicsContext;
	class GameSceneConfig;
	class GameEngine
	{
	public:
		GameEngine();

		void initialize(const EngineConfig& engineConfig);
		void run();

		void loadScene(const GameSceneConfig* config);
		void unloadCurrentScene();

		const std::string& getCurrentScene() const;
		EngineStorage* getEngineStorage() const;

	private:
		void initializeInput(const KeyboardConfig& keyboardConfig, const ControllerConfig& controllerConfig);
		void onWindowUpdate(GLfloat deltaTime);
		void onWindowResize(const Size& size);
		std::unique_ptr<GameInstance> buildGameInstance(const GameInstanceConfig* config);

		std::vector<std::unique_ptr<GameInstance>> m_spriteInstances;
		std::vector<std::unique_ptr<GameInstance>> m_collider2DInstances;
		std::unique_ptr<GraphicsContext> m_graphicsContext{ nullptr };
		std::unique_ptr<EngineStorage> m_engineStorage{ nullptr };
		std::unique_ptr<RenderingSubsystem> m_renderingSubsystem{ nullptr };
		std::unique_ptr<CollisionSolver2D> m_collisionSolver2D{ nullptr };
		std::shared_ptr<FontLibrary> m_fontLibrary{ nullptr };
		std::string m_currentScene{ "Unknown" };
	};
}
