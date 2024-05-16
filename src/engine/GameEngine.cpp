#include <filesystem>
#include <stdexcept>

#include "Camera.h"
#include "ComponentFactory.h"
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
			GL_TRIANGLES,
			"sprite_renderer",
			"sprite");

		/*m_renderingSubsystem->registerRenderer(
			MeshType::COLLIDER,
			GL_DYNAMIC_DRAW,
			GL_LINES,
			{
				VertexAttribute{ "position", 0, 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "color", sizeof(glm::vec3), 3, sizeof(Vertex), GL_FLOAT }
			},
			"collider_2d_renderer",
			"unlit_color");*/

		m_instanceManager = std::make_unique<GameInstanceManager>();

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
		auto sceneIt = m_gameScenes.find(name);

		if (sceneIt == m_gameScenes.cend())
		{
			throw std::runtime_error("Cannot load scene. Scene not found: " + name);
		}

		unloadCurrentScene();

		const auto& sceneConfig = sceneIt->second;

		m_renderingSubsystem->setBackgroundColor(sceneConfig->getBackgroundColor());

		std::vector<GameInstanceConfig*> spriteConfigs;
		std::vector<GameInstanceConfig*> collider2DConfigs;

		for (const auto& gameInstanceConfig : sceneConfig->getInstanceConfigs())
		{
			switch (gameInstanceConfig->getType())
			{
				case (GameInstanceType::SPRITE):
				{
					spriteConfigs.push_back(gameInstanceConfig.get());
					break;
				}
			}
		}

		m_renderingSubsystem->allocateVertexState(
			"sprite_renderer",
			GL_DYNAMIC_DRAW,
			{
				VertexAttribute{ "position", 0, 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "color", sizeof(glm::vec3), 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "textureCoordinate", 2 * sizeof(glm::vec3), 2, sizeof(Vertex), GL_FLOAT }
			});

		for (const auto* spriteConfig : spriteConfigs)
		{
			m_spriteInstances.push_back(buildGameInstance(spriteConfig));
		}

		// TODO: Collider 2D instances as well...

		m_currentScene = name;
	}

	void GameEngine::unloadCurrentScene()
	{
		/*for (const auto& collider2DInstance : m_collider2DInstances)
		{
			m_renderingSubsystem->unregisterRenderObject("collider_2d_renderer", collider2DInstance->getRenderComponents());
			m_instanceManager->unregisterInstance(collider2DInstance->getInternalName());
		}

		m_collider2DInstances.clear();*/

		for (const auto& spriteInstance : m_spriteInstances)
		{
			m_renderingSubsystem->unregisterRenderObject("sprite_renderer", spriteInstance->getRenderComponents());
			m_instanceManager->unregisterInstance(spriteInstance->getInternalName());
		}

		m_renderingSubsystem->releaseVertexState();

		m_spriteInstances.clear();

		m_currentScene.clear();
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindow()->GetHandle());

		GameInstanceManager::updateGameInstances(deltaTime, m_spriteInstances);

		// m_collisionResolver2D->ResolveCollisions();

		GameInstanceManager::updateGameInstances(deltaTime, m_collider2DInstances);

		for (const auto& collider2DInstance : m_collider2DInstances)
		{
			for (const auto& behaviourComponent : collider2DInstance->getBehaviourComponents())
			{
				behaviourComponent->update(deltaTime);
			}
		}

		m_renderingSubsystem->preRender();
		m_renderingSubsystem->render("sprite_renderer", m_spriteInstances);

		/*for (const auto& collider2DInstance : m_collider2DInstances)
		{
			m_renderingSubsystem->render("collider2DRenderer", collider2DInstance->getRenderComponents());
		}*/
	}

	void GameEngine::onWindowResize(const Size& size)
	{
		m_renderingSubsystem->getCamera()->SetAspectRatio(static_cast<float>(size.width) / size.height);

		glViewport(0, 0, size.width, size.height);
	}

	std::unique_ptr<GameInstance> GameEngine::buildGameInstance(const GameInstanceConfig* config)
	{
		std::unique_ptr<GameInstance> gameInstance = std::make_unique<GameInstance>();

		EngineStatus registerStatus = m_instanceManager->registerInstance(gameInstance, config);

		if (!registerStatus)
			throw std::runtime_error("Failed to register game instance: " + config->getName() + " error was: " + registerStatus.message);

		std::vector<RenderComponentPtr> renderComponents = ComponentFactory::createRenderComponents(config);

		switch (config->getType())
		{
		case GameInstanceType::SPRITE:
			registerStatus = m_renderingSubsystem->registerRenderObject("sprite_renderer", renderComponents);
			break;
		// TODO: Colliders!
		default:
			registerStatus = { "Failed to register game instance of unknown type", true };
			break;
		}

		if (!registerStatus)
			throw std::runtime_error("Failed to register game instance: " + config->getName() + " error was: " + registerStatus.message);

		EngineStatus initializeStatus;

		// TODO: Refactor i.e. make a base Component(::initialize) class once everything is replaced and remove duplicate code

		const auto& renderConfigs = config->getRenderConfigs();
		for (size_t i = 0; i < renderConfigs.size(); ++i)
		{
			auto& renderComponent = renderComponents[i];

			initializeStatus = renderComponent->initialize(renderConfigs[i].get());

			if (!initializeStatus)
				throw std::runtime_error("Failed to initialize game instance: " + config->getName() + " error was: " + initializeStatus.message);

			gameInstance->acquireRenderComponent(std::move(renderComponent));
		}

		std::vector<BehaviourComponentPtr> behaviourComponents = ComponentFactory::createBehaviourComponents(config);

		const auto& behaviourConfigs = config->getBehaviourConfigs();
		for (size_t i = 0; i < behaviourConfigs.size(); ++i)
		{
			auto& behaviourComponent = behaviourComponents[i];

			initializeStatus = behaviourComponent->initialize(gameInstance, behaviourConfigs[i].get());

			if (!initializeStatus)
				throw std::runtime_error("Failed to initialize game instance: " + config->getName() + " error was: " + initializeStatus.message);

			gameInstance->acquireBehaviourComponent(std::move(behaviourComponent));
		}

		return gameInstance;
	}
}