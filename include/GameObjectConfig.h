#pragma once

#include <memory>
#include <vector>

#include "ComponentConfig.h"

namespace diamond_engine {
enum class GameObjectType : unsigned {
	SPRITE = 0
};

class GameObjectConfig {
public:
	using ComponentConfigList = std::vector<std::unique_ptr<ComponentConfig>>;

	void AddComponentConfig(std::unique_ptr<ComponentConfig> conmponentConfig);

	const ComponentConfigList& GetComponentConfigs() const;

	void SetType(GameObjectType type);

	GameObjectType GetType() const;

private:
	ComponentConfigList m_componentConfigs{ };
	GameObjectType m_type{ GameObjectType::SPRITE };
};
}
