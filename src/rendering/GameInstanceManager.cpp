#include <stdexcept>

#include "Camera.h"
#include "GameInstanceManager.h"
#include "RenderingSubsystem.h"
#include "SpriteInstanceConfig.h"

namespace diamond_engine
{
	GameInstanceManager::GameInstanceManager(
		const std::shared_ptr<SharedShaderStore>& sharedShaderStore,
		const std::shared_ptr<TextureLoader>& sharedTextureLoader) :
		m_sharedShaderStore(sharedShaderStore),
		m_sharedTextureLoader(sharedTextureLoader),
		m_renderObjectAllocator(std::make_shared<AlignedAllocator<RenderObject, 4>>()),
		m_renderingSubsystem(std::make_shared<RenderingSubsystem>(m_sharedShaderStore)),
		m_spriteInstanceManager(std::make_unique<SpriteInstanceManager>()),
		m_camera(std::make_shared<Camera>())
	{
		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);

		m_spriteInstanceManager->setSharedObjectAllocator(m_renderObjectAllocator);
		m_spriteInstanceManager->setSharedRenderingSubsystem(m_renderingSubsystem);
		m_spriteInstanceManager->setSharedTextureLoader(m_sharedTextureLoader);
		m_spriteInstanceManager->setShaderProgramName("sprite");
		m_spriteInstanceManager->setCamera(m_camera);
	}

	void GameInstanceManager::unloadCurrentScene()
	{
		// Time to free any previously loaded scene objects:
		// TODO: Also free allocated vertex/element buffers

		m_renderingSubsystem->freeAllocatedInstances();
		m_renderObjectAllocator->Free();
	}

	EngineStatus GameInstanceManager::loadScene(const SceneConfig& sceneConfig)
	{
		unloadCurrentScene();

		const size_t maxObjects = static_cast<size_t>(sceneConfig.GetMaxObjects());

		m_renderObjectAllocator->Allocate(maxObjects);
		m_renderObjectAllocator->Expand(maxObjects);

		m_renderingSubsystem->setMaxInstanceCount(maxObjects);

		/*
		for (const auto& gameObjectConfig : sceneConfig.GetGameObjectConfigs())
		{

		}
		*/

		MaterialConfig materialConfig;
		materialConfig.SetTextureName("cat");

		MeshRenderConfig meshRendererConfig;
		meshRendererConfig.SetMeshType(MeshType::QUAD);
		meshRendererConfig.SetDrawMode(GL_STATIC_DRAW);

		std::unique_ptr<SpriteInstanceConfig> spriteInstanceConfig = std::make_unique<SpriteInstanceConfig>();
		spriteInstanceConfig->setMaterialConfig(materialConfig); // Test
		spriteInstanceConfig->setMeshRenderConfig(meshRendererConfig);
		
		EngineStatus allocateStatus = m_spriteInstanceManager->allocateInstance(spriteInstanceConfig.get());
		if (!allocateStatus)
		{
			throw std::runtime_error(allocateStatus.message);
		}

		return { };
	}
}