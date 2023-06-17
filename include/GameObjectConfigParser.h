#pragma once

#include <memory>

#include "GameObjectConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class GameObjectConfigParser {
public:
	std::unique_ptr<GameObjectConfig> Parse(const pugi::xml_node& gameObjectNode);
};
}
