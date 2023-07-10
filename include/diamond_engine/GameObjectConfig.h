#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>

#include "GameObjectType.h"
#include "ComponentConfig.h"
#include "BehaviourConfig.h"

namespace diamond_engine {
class GameObjectConfig {
public:
	using ComponentConfigList = std::vector<std::unique_ptr<ComponentConfig>>;
	using BehaviourConfigList = std::vector<std::unique_ptr<BehaviourConfig>>;

	void AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig);

	const ComponentConfigList& GetComponentConfigs() const;

	void AddBehaviourConfig(std::unique_ptr<BehaviourConfig> behaviourConfig);

	const BehaviourConfigList& GetBehaviourConfigs() const;

	void SetType(GameObjectType type);

	void SetPosition(const glm::vec3& position);

	const glm::vec3& GetPosition() const;

	void SetColor(const glm::vec3& color);

	const glm::vec3& GetColor() const;

	GameObjectType GetType() const;

private:
	ComponentConfigList m_componentConfigs{ };
	BehaviourConfigList m_behaviourConfigs{ };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
	GameObjectType m_type{ GameObjectType::SPRITE };
};
}
