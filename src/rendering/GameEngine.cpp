#include <filesystem>
#include <stdexcept>

#include "GameEngine.h"
#include "GameSceneConfigParser.h"
#include "Input.h"
#include "SharedShaderStore.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	GameEngine::GameEngine() :
		m_graphicsContext(std::make_unique<GraphicsContext>()),
		m_shaderStore(std::make_shared<SharedShaderStore>()),
		m_textureLoader(std::make_shared<TextureLoader>()) { }

	void GameEngine::initialize(const EngineConfig& engineConfig)
	{
		m_graphicsContext->Initialize(
			engineConfig,
			std::bind(&GameEngine::onWindowUpdate, this, std::placeholders::_1),
			std::bind(&GameEngine::onWindowResize, this, std::placeholders::_1));

		m_shaderStore->Load(engineConfig.getShadersDirectory());
		m_textureLoader->Load(engineConfig.getTexturesDirectory());

		m_instanceManager		= std::make_unique<GameInstanceManager>(m_textureLoader);
		m_renderingSubsystem	= std::make_unique<RenderingSubsystem>(m_shaderStore);
		m_renderingSubsystem->registerRenderer("sprite");

		initializeInput(engineConfig.GetKeyboardConfig(), engineConfig.getControllerConfig());
	}

	void GameEngine::run()
	{
		m_graphicsContext->Execute();
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
		auto sceneConfig = GameSceneConfigParser::parseFile(file, &parseStatus);

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

			m_currentScene.clear();
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
		m_renderingSubsystem->setMaxInstanceCount(sceneConfig->getMaxInstanceCount());
		EngineStatus loadStatus = m_instanceManager->loadScene((*sceneConfig.get()), m_renderingSubsystem);

		if (!loadStatus)
		{
			throw std::runtime_error("Cannot load scene. Error was: " + loadStatus.message);
		}

		m_currentScene = name;
	}

	void GameEngine::unloadCurrentScene()
	{
		m_instanceManager->unloadCurrentScene();
		m_renderingSubsystem->freeAllocatedInstances();
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindowHandle());

		// TODO: Behaviours

		m_renderingSubsystem->render("sprite"); // TODO: Have this configurable i.e. do we want to also render colliders
	}

	void GameEngine::onWindowResize(const Size& size)
	{
		// TODO! Change the aspect radio of the rendering subsystem shared camera

		glViewport(0, 0, size.width, size.height);
	}
}