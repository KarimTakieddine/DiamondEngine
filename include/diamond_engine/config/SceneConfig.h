#pragma once

#include <GL/glew.h>

#include "GameObjectConfig.h"

namespace diamond_engine {
class SceneConfig {
public:
	using GameObjectConfigList = std::vector<std::unique_ptr<GameObjectConfig>>;

	void AddGameObjectConfig(std::unique_ptr<GameObjectConfig> gameObjectConfig);

	const GameObjectConfigList& GetGameObjectConfigs() const;

	void SetMaxObjects(GLint maxObjects);

	GLint GetMaxObjects() const;

private:
	GameObjectConfigList m_gameObjectConfigs{ };
	GLint m_maxObjects{ 0 };
};
}
