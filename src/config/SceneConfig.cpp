#include "SceneConfig.h"

namespace diamond_engine {
	void SceneConfig::AddGameObjectConfig(std::unique_ptr<GameObjectConfig> gameObjectConfig) {
		m_gameObjectConfigs.push_back(std::move(gameObjectConfig));
	}

	const SceneConfig::GameObjectConfigList& SceneConfig::GetGameObjectConfigs() const {
		return m_gameObjectConfigs;
	}

	void SceneConfig::SetMaxObjects(GLint maxObjects) {
		m_maxObjects = maxObjects;
	}

	GLint SceneConfig::GetMaxObjects() const {
		return m_maxObjects;
	}
}