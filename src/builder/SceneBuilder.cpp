#include <stdexcept>

#include "GameObjectBuilder.h"
#include "SceneBuilder.h"
#include "SceneConfig.h"

namespace diamond_engine {
	void SceneBuilder::SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator) {
		m_bufferAllocator = bufferAllocator;
	}

	void SceneBuilder::SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) {
		m_sharedShaderStore = sharedShaderStore;
	}

	void SceneBuilder::SetSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader) {
		m_sharedTextureLoader = sharedTextureLoader;
	}

	void SceneBuilder::SetSharedSpriteSheetLoader(const std::shared_ptr<SpriteSheetLoader>& sharedSpriteSheetLoader) {
		m_sharedSpriteSheetLoader = sharedSpriteSheetLoader;
	}

	std::unique_ptr<Scene> SceneBuilder::Build(const SceneConfig* sceneConfig) {
		if (!m_sharedShaderStore) {
			throw std::runtime_error("Cannot build Scene instance without shared shader store");
		}

		if (!m_sharedTextureLoader) {
			throw std::runtime_error("Cannot build Scene instance without shared texture loader");
		}

		std::unique_ptr<Scene> result = std::make_unique<Scene>(m_sharedShaderStore, m_sharedTextureLoader, m_sharedSpriteSheetLoader);

		result->SetMaxObjects(sceneConfig->GetMaxObjects());

		SetBufferAllocator(result->GetBufferAllocator());

		const auto& gameObjectConfigs = sceneConfig->GetGameObjectConfigs();

		for (size_t i = 0; i < gameObjectConfigs.size(); ++i) {
			if (!m_bufferAllocator) {
				throw std::runtime_error("Null or no buffer allocator set on SceneBuilder instance");
			}

			const GameObjectConfig* gameObjectConfig	= gameObjectConfigs[i].get();
			const std::string& gameObjectName			= gameObjectConfig->getName();

			std::unique_ptr<GameObject> gameObject = GameObjectBuilder::Build(m_bufferAllocator, gameObjectConfig);

			gameObject->setName(gameObjectName.empty() ? "GameObject_" + std::to_string(i) : gameObjectName);

			result->AddGameObject(
				std::move(gameObject),
				gameObjectConfig->GetMaterialConfig(),
				gameObjectConfig->GetPosition(),
				gameObjectConfig->GetScale(),
				gameObjectConfig->GetType());
		}

		return result;
	}
}