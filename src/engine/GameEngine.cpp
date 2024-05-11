#include <filesystem>
#include <stdexcept>

#include "Camera.h"
#include "GameEngine.h"
#include "GameSceneConfigParser.h"
#include "Input.h"
#include "SharedMeshStore.h"
#include "SharedShaderStore.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	GameEngine::GameEngine() :
		m_graphicsContext(std::make_unique<GraphicsContext>())
		{ }

	void GameEngine::initialize(const EngineConfig& engineConfig)
	{
		m_graphicsContext->Initialize(
			engineConfig,
			std::bind(&GameEngine::onWindowUpdate, this, std::placeholders::_1),
			std::bind(&GameEngine::onWindowResize, this, std::placeholders::_1));

		SharedShaderStore::getInstance()->Load(engineConfig.getShadersDirectory());
		TextureLoader::getInstance()->Load(engineConfig.getTexturesDirectory());
		SharedMeshStore::getInstance()->loadMeshes();

		m_renderingSubsystem = std::make_unique<RenderingSubsystem>();
		m_renderingSubsystem->setMaxRendererCount(2);
		m_renderingSubsystem->registerRenderer(
			MeshType::QUAD,
			GL_DYNAMIC_DRAW,
			GL_TRIANGLES,
			{
				VertexAttribute{ "position", 0, 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "color", sizeof(glm::vec3), 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "textureCoordinate", 2 * sizeof(glm::vec3), 2, sizeof(Vertex), GL_FLOAT }
			},
			"sprite");

		/*
		m_renderingSubsystem->registerRenderer(
			MeshType::COLLIDER,
			GL_DYNAMIC_DRAW,
			GL_LINES,
			{
				VertexAttribute{ "position", 0, 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "color", sizeof(glm::vec3), 3, sizeof(Vertex), GL_FLOAT }
			},
			"unlit_color");
		*/

		m_instanceManager = std::make_unique<GameInstanceManager>();
		m_instanceManager->setRenderingSubsystem(m_renderingSubsystem);

		initializeInput(engineConfig.GetKeyboardConfig(), engineConfig.getControllerConfig());

		onWindowResize(m_graphicsContext->getWindow()->getCurrentSize());
	}

	void GameEngine::run()
	{
		m_graphicsContext->Execute();

		unloadCurrentScene();
		m_renderingSubsystem->freeAllocatedRenderers();
		SharedMeshStore::getInstance()->unloadMeshes();
		TextureLoader::getInstance()->unloadTextures();
		SharedShaderStore::getInstance()->unload();
	}

	void GameEngine::initializeInput(const KeyboardConfig& keyboardConfig, const ControllerConfig& controllerConfig)
	{
		auto& stateMonitor = input::StateMonitor::GetInstance();

		for (const auto& keyConfig : keyboardConfig.GetKeyConfigs()) {
			stateMonitor.RegisterKeyboardKey(keyConfig.name, keyConfig.code);
		}

		for (const auto& buttonConfig : controllerConfig.getButtonConfigs())
		{
			stateMonitor.RegisterControllerButton(buttonConfig.name, buttonConfig.button);
		}

		for (const auto& joystickConfig : controllerConfig.getJoystickConfigs())
		{
			stateMonitor.RegisterControllerJoystick(joystickConfig.name, joystickConfig.joystick);
		}

		stateMonitor.setJoystickDeadzone(controllerConfig.getJoystickDeadzone());
	}

	void GameEngine::addScene(const std::string& name, const std::string& file)
	{
		std::filesystem::path scenePath(file);
		scenePath.make_preferred();

		if (!std::filesystem::exists(scenePath) || !std::filesystem::is_regular_file(scenePath))
		{
			throw std::runtime_error("GameConfigScene file not found: " + scenePath.string());
		}

		EngineStatus parseStatus;
		auto sceneConfig = parseSceneFile(file, &parseStatus);

		if (!parseStatus)
		{
			// TODO: Revisit general error handling logic!!!
			throw std::runtime_error("Failed to add GameSceneConfig. A parsing error was detected: " + parseStatus.message);
		}

		addScene(name, std::move(sceneConfig));
	}

	void GameEngine::addScene(const std::string& name, std::unique_ptr<GameSceneConfig> sceneConfig)
	{
		m_gameScenes.insert({ name, std::move(sceneConfig) });
	}

	void GameEngine::removeScene(const std::string& name)
	{
		if (name == m_currentScene)
		{
			unloadCurrentScene();
		}

		if (m_gameScenes.find(name) == m_gameScenes.cend())
		{
			return;
		}

		m_gameScenes.erase(name);
	}

	void GameEngine::loadScene(const std::string& name)
	{
		unloadCurrentScene();

		auto sceneIt = m_gameScenes.find(name);

		if (sceneIt == m_gameScenes.cend())
		{
			throw std::runtime_error("Cannot load scene. Scene not found: " + name);
		}

		const auto& sceneConfig = sceneIt->second;
		EngineStatus loadStatus = m_instanceManager->loadScene(*sceneConfig.get());

		if (!loadStatus)
		{
			throw std::runtime_error("Cannot load scene. Error was: " + loadStatus.message);
		}

		m_currentScene			= name;
		m_sceneBackgroundColor	= sceneConfig->getBackgroundColor();
	}

	void GameEngine::unloadCurrentScene()
	{
		m_renderingSubsystem->freeRegisteredInstructions();
		m_instanceManager->unloadCurrentScene();
		m_currentScene.clear();
		m_sceneBackgroundColor = { };
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindow()->GetHandle());

		// TODO: Behaviours and background color config

		glClearColor(m_sceneBackgroundColor.x, m_sceneBackgroundColor.y, m_sceneBackgroundColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_renderingSubsystem->renderAll(); // TODO: Have this configurable i.e. do we want to also render colliders
	}

	void GameEngine::onWindowResize(const Size& size)
	{
		m_renderingSubsystem->getCamera()->SetAspectRatio(static_cast<float>(size.width) / size.height);

		glViewport(0, 0, size.width, size.height);
	}
}