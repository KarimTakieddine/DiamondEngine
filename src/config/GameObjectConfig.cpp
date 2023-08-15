#include "GameObjectConfig.h"

namespace diamond_engine {
	void GameObjectConfig::AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig) {
		m_componentConfigs.push_back(std::move(conmponentConfig));
	}

	const GameObjectConfig::ComponentConfigList& GameObjectConfig::GetComponentConfigs() const {
		return m_componentConfigs;
	}

	void GameObjectConfig::AddBehaviourConfig(std::unique_ptr<BehaviourConfig> behaviourConfig) {
		m_behaviourConfigs.push_back(std::move(behaviourConfig));
	}

	const GameObjectConfig::BehaviourConfigList& GameObjectConfig::GetBehaviourConfigs() const {
		return m_behaviourConfigs;
	}

	const MaterialConfig& GameObjectConfig::GetMaterialConfig() const {
		return m_materialConfig;
	}

	void GameObjectConfig::SetMaterialConfig(const MaterialConfig& materialConfig) {
		m_materialConfig = materialConfig;
	}
	
	void GameObjectConfig::SetType(GameObjectType type) {
		m_type = type;
	}

	void GameObjectConfig::SetPosition(const glm::vec3& position) {
		m_position = position;
	}

	const glm::vec3& GameObjectConfig::GetPosition() const {
		return m_position;
	}
	
	GameObjectType GameObjectConfig::GetType() const {
		return m_type;
	}
}