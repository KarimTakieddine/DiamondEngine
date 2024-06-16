#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include "AudioEngine.h"
#include "Camera.h"
#include "Collider2DComponent.h"
#include "Collider2DComponentConfig.h"
#include "ComponentFactory.h"
#include "Debugger.h"
#include "EngineMacros.h"
#include "FontEngine.h"
#include "FontLibrary.h"
#include "GameEngine.h"
#include "GameSceneConfigParser.h"
#include "GLAllocationEventHandler.h"
#include "GLMemoryRequestHandler.h"
#include "SpriteAnimationPlayHandler.h"
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

		Debugger::getInstance()->registerHandler(DebugEvent::Type::GL_OBJECT_ALLOCATION, std::make_unique<GLAllocationEventHandler>());
		Debugger::getInstance()->registerHandler(DebugEvent::Type::GL_OBJECT_REQUEST, std::make_unique<GLMemoryRequestHandler>());
		Debugger::getInstance()->registerHandler(DebugEvent::Type::SPRITE_ANIMATION_PLAY, std::make_unique<SpriteAnimationPlayHandler>());

		SharedShaderStore::getInstance()->Load(engineConfig.getShadersDirectory());
		TextureLoader::getInstance()->Load(engineConfig.getTexturesDirectory());
		SpriteSheetLoader::getInstance()->load(engineConfig.getSpriteSheetsDirectory());
		SharedMeshStore::getInstance()->setMaxMeshCount(16);
		SharedMeshStore::getInstance()->loadMeshes();

		m_renderingSubsystem = std::make_unique<RenderingSubsystem>();
		m_renderingSubsystem->setMaxRendererCount(3);
		m_renderingSubsystem->registerRenderer(
			MeshType::QUAD,
			GL_TRIANGLES,
			"font_renderer",
			"font");

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

		// TODO: Move this to a config
		m_fontLibrary = std::make_shared<FontLibrary>();
		m_fontLibrary->registerFont('a', { 0, 0 });
		m_fontLibrary->registerFont('b', { 0, 1 });
		m_fontLibrary->registerFont('c', { 0, 2 });
		m_fontLibrary->registerFont('d', { 0, 3 });
		m_fontLibrary->registerFont('e', { 0, 4 });
		m_fontLibrary->registerFont('f', { 0, 5 });
		m_fontLibrary->registerFont('g', { 0, 6 });
		m_fontLibrary->registerFont('h', { 0, 7 });
		m_fontLibrary->registerFont('i', { 0, 8 });
		m_fontLibrary->registerFont('j', { 0, 9 });
		m_fontLibrary->registerFont('k', { 0, 10 });
		m_fontLibrary->registerFont('l', { 0, 11 });
		m_fontLibrary->registerFont('m', { 0, 12 });
		m_fontLibrary->registerFont('n', { 0, 13 });
		m_fontLibrary->registerFont('o', { 0, 14 });
		m_fontLibrary->registerFont('p', { 0, 15 });
		m_fontLibrary->registerFont('q', { 1, 0 });
		m_fontLibrary->registerFont('r', { 1, 1 });
		m_fontLibrary->registerFont('s', { 1, 2 });
		m_fontLibrary->registerFont('t', { 1, 3 });
		m_fontLibrary->registerFont('u', { 1, 4 });
		m_fontLibrary->registerFont('v', { 1, 5 });
		m_fontLibrary->registerFont('w', { 1, 6 });
		m_fontLibrary->registerFont('x', { 1, 7 });
		m_fontLibrary->registerFont('y', { 1, 8 });
		m_fontLibrary->registerFont('z', { 1, 9 });
		m_fontLibrary->registerFont('0', { 1, 10 });
		m_fontLibrary->registerFont('1', { 1, 11 });
		m_fontLibrary->registerFont('2', { 1, 12 });
		m_fontLibrary->registerFont('3', { 1, 13 });
		m_fontLibrary->registerFont('4', { 1, 14 });
		m_fontLibrary->registerFont('5', { 1, 15 });
		m_fontLibrary->registerFont('6', { 2, 0 });
		m_fontLibrary->registerFont('7', { 2, 1 });
		m_fontLibrary->registerFont('8', { 2, 2 });
		m_fontLibrary->registerFont('9', { 2, 3 });
		m_fontLibrary->registerFont(' ', { 2, 4 });

		FontEngine::getInstance()->setFontLibrary(m_fontLibrary);

		FontEngine::getInstance()->registerTextWindow(
			{ 64, 64 },
			{ 64, 64 },
			TextureLoader::getInstance()->GetTexture("ascii_fonts_green"));

		m_instanceManager	= std::make_unique<GameInstanceManager>();
		m_collisionSolver2D = std::make_unique<CollisionSolver2D>();

		initializeInput(engineConfig.GetKeyboardConfig(), engineConfig.getControllerConfig());

		onWindowResize(m_graphicsContext->getWindow()->getCurrentSize());
	}

	void GameEngine::run()
	{
		m_graphicsContext->Execute();

		unloadCurrentScene();

		/*
			TODO: Find a way to unload resources while the game
			is still active so we can see debug events ?
		*/

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

		FontEngine::getInstance()->allocateGraphicsMemory(m_renderingSubsystem);
		FontEngine::getInstance()->setWindowDimensions(0, { -0.95f, 0.94f }, { 0.45f, 0.45f });
		FontEngine::getInstance()->setWindowColor(0, { 1.0f, 1.0f, 1.0f });

		m_currentScene = name;
	}

	void GameEngine::unloadCurrentScene()
	{
		FontEngine::getInstance()->releaseGraphicsMemory(m_renderingSubsystem);

		for (auto collider2DIt = m_collider2DInstances.rbegin(); collider2DIt != m_collider2DInstances.rend(); ++collider2DIt)
		{
			m_renderingSubsystem->unregisterRenderObject("collider_2d_renderer", (*collider2DIt)->getRenderComponents());
			m_instanceManager->unregisterInstance((*collider2DIt)->getInternalName());
		}

		m_renderingSubsystem->releaseVertexState();
		m_collider2DInstances.clear();

		for (auto spriteIt = m_spriteInstances.rbegin(); spriteIt != m_spriteInstances.rend(); ++spriteIt)
		{
			m_renderingSubsystem->unregisterRenderObject("sprite_renderer", (*spriteIt)->getRenderComponents());
			m_instanceManager->unregisterInstance((*spriteIt)->getInternalName());
		}

		m_renderingSubsystem->releaseVertexState();
		m_spriteInstances.clear();

		m_collisionSolver2D->clear();

		m_currentScene.clear();
	}

	void GameEngine::onWindowUpdate(GLfloat deltaTime)
	{
		DEBUG_EXEC(Debugger::getInstance()->handleAllEvents());

		input::StateMonitor::GetInstance().MonitorStates(m_graphicsContext->getWindow()->GetHandle());

		GameInstanceManager::updateGameInstances(deltaTime, m_spriteInstances);
		m_collisionSolver2D->ResolveCollisions();
		GameInstanceManager::updateGameInstances(deltaTime, m_collider2DInstances);

		m_renderingSubsystem->preRender();
		m_renderingSubsystem->render("sprite_renderer", m_spriteInstances);
		m_renderingSubsystem->render("collider_2d_renderer", m_collider2DInstances);

		FontEngine::getInstance()->render(m_renderingSubsystem);
	}

	void GameEngine::onWindowResize(const Size& size)
	{
		if (size.width <= 0 || size.height <= 0)
			return;

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

		gameInstance->setActive(true);

		return gameInstance;
	}
}