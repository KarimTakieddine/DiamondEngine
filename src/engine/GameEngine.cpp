#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include "AudioEngine.h"
#include "Camera.h"
#include "Collider2DComponent.h"
#include "Collider2DComponentConfig.h"
#include "ComponentFactory.h"
#include "GameEngine.h"
#include "GameSceneConfigParser.h"
#include "Input.h"
#include "MaterialComponentConfig.h"
#include "SharedMeshStore.h"
#include "SharedShaderStore.h"
#include "SpriteSheetLoader.h"
#include "TextureLoader.h"
#include "TransformComponentConfig.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	GameEngine::GameEngine() :
		m_graphicsContext(std::make_unique<GraphicsContext>())
		{ }

	void GameEngine::initialize(const EngineConfig& engineConfig)
	{
		AudioEngine::getInstance()->initialize(engineConfig.getAudioDirectory());

		m_graphicsContext->Initialize(
			engineConfig,
			std::bind(&GameEngine::onWindowUpdate, this, std::placeholders::_1),
			std::bind(&GameEngine::onWindowResize, this, std::placeholders::_1));

		SharedShaderStore::getInstance()->Load(engineConfig.getShadersDirectory());
		TextureLoader::getInstance()->Load(engineConfig.getTexturesDirectory());
		SpriteSheetLoader::getInstance()->load(engineConfig.getSpriteSheetsDirectory());
		SharedMeshStore::getInstance()->loadMeshes();

		m_renderingSubsystem = std::make_unique<RenderingSubsystem>();
		m_renderingSubsystem->setMaxRendererCount(2);
		m_renderingSubsystem->registerRenderer(
			MeshType::QUAD,
			GL_TRIANGLES,
			"sprite_renderer",
			"sprite");

		m_renderingSubsystem->registerRenderer(
			MeshType::COLLIDER,
			GL_LINES,
			"collider_2d_renderer",
			"unlit_color");

		m_instanceManager	= std::make_unique<GameInstanceManager>();
		m_collisionSolver2D = std::make_unique<CollisionSolver2D>();

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
		std::vector<std::unique_ptr<GameInstanceConfig>> collider2DConfigs;

		const auto& gameInstanceConfigs = sceneConfig->getInstanceConfigs();
		for (GLsizei i = 0; i < gameInstanceConfigs.size(); ++i)
		{
			const auto& gameInstanceConfig = gameInstanceConfigs[i];
			switch (gameInstanceConfig->getType())
			{
				case (GameInstanceType::SPRITE):
				{
					auto& behaviourConfigs = gameInstanceConfig->getBehaviourConfigs();
					auto collider2DIt = std::find_if(
						behaviourConfigs.begin(),
						behaviourConfigs.end(),
						[](const auto& behaviourConfig) { return std::string(behaviourConfig->getName()) == "Collider2D"; });

					if (collider2DIt != behaviourConfigs.cend())
					{
						std::unique_ptr<GameInstanceConfig> colliderConfig = std::make_unique<GameInstanceConfig>();

						colliderConfig->setType(GameInstanceType::COLLIDER_2D);

						std::unique_ptr<TransformComponentConfig> transformConfig = std::make_unique<TransformComponentConfig>();

						const auto& renderConfigs = gameInstanceConfig->getRenderConfigs();
						auto transformConfigIt = std::find_if(
							renderConfigs.cbegin(),
							renderConfigs.cend(),
							[](const auto& renderConfig) { return std::string(renderConfig->getName()) == "Transform"; });

						transformConfig->setPosition(
							transformConfigIt == renderConfigs.cend() ?
							glm::vec3{ 0.0f, 0.0f, 0.0f } : dynamic_cast<const TransformComponentConfig*>(transformConfigIt->get())->getPosition());

						colliderConfig->addRenderConfig(std::move(transformConfig));

						std::unique_ptr<MaterialComponentConfig> materialConfig = std::make_unique<MaterialComponentConfig>();
						materialConfig->setColor({ 0.0f, 1.0f, 0.0f });
						colliderConfig->addRenderConfig(std::move(materialConfig));

						Collider2DComponentConfig* colliderComponentConfig = dynamic_cast<Collider2DComponentConfig*>(collider2DIt->get());
						colliderComponentConfig->setTargetIndex(i);
						std::unique_ptr<Collider2DComponentConfig> copy = std::make_unique<Collider2DComponentConfig>();
						copy->setSize(colliderComponentConfig->getSize());
						copy->setType(colliderComponentConfig->getType());
						copy->setTargetIndex(colliderComponentConfig->getTargetIndex());
						colliderConfig->addBehaviourConfig(std::move(copy));
						
						const std::string& colliderName = gameInstanceConfig->getName() + "_collider_";
						int colliderCount = std::count_if(
							collider2DConfigs.cbegin(),
							collider2DConfigs.cend(),
							[colliderName](const auto& c) { return c->getName() == colliderName; });
						colliderConfig->setName(colliderName + std::to_string(colliderCount));

						collider2DConfigs.push_back(std::move(colliderConfig));
					}

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

		m_renderingSubsystem->allocateVertexState(
			"collider_2d_renderer",
			GL_DYNAMIC_DRAW,
			{
				VertexAttribute{ "position", 0, 3, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "color", sizeof(glm::vec3), 3, sizeof(Vertex), GL_FLOAT }
			});

		for (const auto& collider2DConfig : collider2DConfigs)
		{
			auto collider2DInstance = buildGameInstance(collider2DConfig.get());

			// TODO: This needs to change...
			const Collider2DComponentConfig* colliderConfig = dynamic_cast<const Collider2DComponentConfig*>(collider2DConfig->getBehaviourConfigs().back().get());
			GameInstance* targetSpriteInstance				= m_spriteInstances[static_cast<size_t>(colliderConfig->getTargetIndex())].get();

			Collider2DComponent* collider2D = collider2DInstance->getBehaviourComponent<Collider2DComponent>("Collider2D");
			collider2D->setTarget(targetSpriteInstance->getRenderComponent<TransformRenderComponent>("Transform"));
			collider2D->setSource(collider2DInstance->getRenderComponent<TransformRenderComponent>("Transform"));

			switch (collider2D->getType())
			{
			case ColliderType::OBSTACLE:
				m_collisionSolver2D->addObstacle(targetSpriteInstance, collider2D);
				break;
			case ColliderType::CHARACTER:
				m_collisionSolver2D->addCharacter(targetSpriteInstance, collider2D);
				break;
			}

			m_collider2DInstances.push_back(std::move(collider2DInstance));
		}

		m_currentScene = name;
	}

	void GameEngine::unloadCurrentScene()
	{
		for (const auto& collider2DInstance : m_collider2DInstances)
		{
			m_renderingSubsystem->unregisterRenderObject("collider_2d_renderer", collider2DInstance->getRenderComponents());
			m_instanceManager->unregisterInstance(collider2DInstance->getInternalName());
		}

		m_renderingSubsystem->releaseVertexState();
		m_collider2DInstances.clear();

		for (const auto& spriteInstance : m_spriteInstances)
		{
			m_renderingSubsystem->unregisterRenderObject("sprite_renderer", spriteInstance->getRenderComponents());
			m_instanceManager->unregisterInstance(spriteInstance->getInternalName());
		}

		m_renderingSubsystem->releaseVertexState();
		m_spriteInstances.clear();

		m_collisionSolver2D->clear();

		m_currentScene.clear();
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindow()->GetHandle());

		GameInstanceManager::updateGameInstances(deltaTime, m_spriteInstances);
		m_collisionSolver2D->ResolveCollisions();
		GameInstanceManager::updateGameInstances(deltaTime, m_collider2DInstances);

		m_renderingSubsystem->preRender();
		m_renderingSubsystem->render("sprite_renderer", m_spriteInstances);
		m_renderingSubsystem->render("collider_2d_renderer", m_collider2DInstances);
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
		case GameInstanceType::COLLIDER_2D:
			registerStatus = m_renderingSubsystem->registerRenderObject("collider_2d_renderer", renderComponents);
			break;
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

		std::vector<BehaviourComponentPtr> behaviourComponents	= ComponentFactory::createBehaviourComponents(config);
		const auto& behaviourConfigs = config->getBehaviourConfigs();

		for (auto& behaviourComponent : behaviourComponents)
		{
			const BehaviourComponentConfig* behaviourConfig = std::find_if(
				behaviourConfigs.cbegin(),
				behaviourConfigs.cend(),
				[&behaviourComponent](const auto& componentConfig) {
					return std::string(componentConfig->getName()) == std::string(behaviourComponent->getName());
				})->get();

			behaviourComponent->setGameInstance(gameInstance.get());
			initializeStatus = behaviourComponent->initialize(behaviourConfig);

			if (!initializeStatus)
				throw std::runtime_error("Failed to initialize game instance: " + config->getName() + " error was: " + initializeStatus.message);

			gameInstance->acquireBehaviourComponent(std::move(behaviourComponent));
		}

		return gameInstance;
	}
}