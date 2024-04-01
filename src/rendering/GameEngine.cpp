#include "GameEngine.h"
#include "Input.h"
#include "SharedShaderStore.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	GameEngine::GameEngine() :
		m_graphicsContext(std::make_unique<GraphicsContext>()),
		m_shaderStore(std::make_shared<SharedShaderStore>()),
		m_textureLoader(std::make_shared<TextureLoader>()),
		m_instanceManager(std::make_unique<GameInstanceManager>(m_textureLoader)),
		m_renderingSubsystem(std::make_unique<RenderingSubsystem>(m_shaderStore))
	{
		m_renderingSubsystem->registerRenderer("sprite"); // TODO: Possibly inside initialize?
	}

	void GameEngine::initialize(const EngineConfig& engineConfig)
	{
		m_graphicsContext->Initialize(
			engineConfig,
			std::bind(&GameEngine::onWindowUpdate, this, std::placeholders::_1),
			std::bind(&GameEngine::onWindowResize, this, std::placeholders::_1));

		m_shaderStore->Load(engineConfig.getShadersDirectory());
		m_textureLoader->Load(engineConfig.getTexturesDirectory());

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

	}

	void GameEngine::addScene(const std::string& name, std::unique_ptr<GameSceneConfig> sceneConfig)
	{

	}

	void GameEngine::removeScene(const std::string& name)
	{

	}

	void GameEngine::loadScene(const std::string& name)
	{
		m_instanceManager->unloadCurrentScene();
		unloadCurrentScene();


	}

	void GameEngine::unloadCurrentScene()
	{
		m_renderingSubsystem->freeAllocatedInstances();
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindowHandle());



		m_renderingSubsystem->renderAll();
	}

	void GameEngine::onWindowResize(const Size& size)
	{
		// TODO!

		glViewport(0, 0, size.width, size.height);
	}
}