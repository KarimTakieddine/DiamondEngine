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

	void GameObjectConfig::SetPosition(const glm::vec3& position) {
		m_position = position;
	}

	const glm::vec3& GameObjectConfig::GetPosition() const {
		return m_position;
	}

	void GameObjectConfig::SetColor(const glm::vec3& color) {
		m_color = color;
	}

	const glm::vec3& GameObjectConfig::GetColor() const {
		return m_color;
	}
	
	GameObjectType GameObjectConfig::GetType() const {
		return m_type;
	}
}