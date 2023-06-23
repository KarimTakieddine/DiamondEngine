#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>

#include "GameObjectType.h"
#include "ComponentConfig.h"

namespace diamond_engine {
class GameObjectConfig {
public:
	using ComponentConfigList = std::vector<std::unique_ptr<ComponentConfig>>;

	void AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig);

	const ComponentConfigList& GetComponentConfigs() const;

	void SetType(GameObjectType type);

	GameObjectType GetType() const;

private:
	ComponentConfigList m_componentConfigs{ };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
	GameObjectType m_type{ GameObjectType::SPRITE };
};
}
