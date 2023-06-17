#include "GameObjectConfig.h"

namespace diamond_engine {
	void GameObjectConfig::AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig) {
		m_componentConfigs.push_back(std::move(conmponentConfig));
	}

	const GameObjectConfig::ComponentConfigList& GameObjectConfig::GetComponentConfigs() const {
		return m_componentConfigs;
	}
	
	void GameObjectConfig::SetType(GameObjectType type) {
		m_type = type;
	}
	
	GameObjectType GameObjectConfig::GetType() const {
		return m_type;
	}
}