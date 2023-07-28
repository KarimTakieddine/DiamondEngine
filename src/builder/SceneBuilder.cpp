#include <stdexcept>

#include "builder/GameObjectBuilder.h"
#include "builder/SceneBuilder.h"
#include "config/SceneConfig.h"

namespace diamond_engine {
	void SceneBuilder::SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator) {
		m_bufferAllocator = bufferAllocator;
	}

	void SceneBuilder::SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) {
		m_sharedShaderStore = sharedShaderStore;
	}

	std::unique_ptr<Scene> SceneBuilder::Build(const SceneConfig* sceneConfig) {
		if (!m_sharedShaderStore) {
			throw std::runtime_error("Cannot build Scene instance without shared shader store");
		}

		std::unique_ptr<Scene> result = std::make_unique<Scene>(m_sharedShaderStore);

		result->SetMaxObjects(sceneConfig->GetMaxObjects());

		SetBufferAllocator(result->GetBufferAllocator());

		for (const auto& gameObjectConfig : sceneConfig->GetGameObjectConfigs()) {
			if (!m_bufferAllocator) {
				throw std::runtime_error("Null or no buffer allocator set on SceneBuilder instance");
			}

			result->AddGameObject(
				GameObjectBuilder::Build(m_bufferAllocator, gameObjectConfig.get()),
				gameObjectConfig->GetPosition(),
				gameObjectConfig->GetColor(),
				gameObjectConfig->GetType());
		}

		return result;
	}
}