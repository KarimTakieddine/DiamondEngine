#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BehaviourConfig.h"
#include "ComponentConfig.h"
#include "MaterialConfig.h"
#include "GameObjectType.h"

namespace diamond_engine {
class GameObjectConfig {
public:
	using ComponentConfigList = std::vector<std::unique_ptr<ComponentConfig>>;
	using BehaviourConfigList = std::vector<std::unique_ptr<BehaviourConfig>>;

	void AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig);

	const ComponentConfigList& GetComponentConfigs() const;

	void AddBehaviourConfig(std::unique_ptr<BehaviourConfig> behaviourConfig);

	const BehaviourConfigList& GetBehaviourConfigs() const;

	const MaterialConfig& GetMaterialConfig() const;

	void SetMaterialConfig(const MaterialConfig& materialConfig);

	void SetType(GameObjectType type);

	void SetPosition(const glm::vec3& position);

	const glm::vec3& GetPosition() const;

	void SetScale(const glm::vec3& scale);

	const glm::vec3& GetScale() const;

	GameObjectType GetType() const;

	const std::string& getName() const;

	void setName(const std::string& name);

private:
	ComponentConfigList m_componentConfigs{ };
	BehaviourConfigList m_behaviourConfigs{ };
	MaterialConfig m_materialConfig{ };
	std::string m_name{ "" };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
	GameObjectType m_type{ GameObjectType::SPRITE };
};
}
