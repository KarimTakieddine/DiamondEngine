#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "GameObjectConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class GameObjectConfigParser {
public:
	using ComponentParseFunc = std::function<std::unique_ptr<ComponentConfig>(const pugi::xml_node&)>;

	static std::unordered_map<std::string, ComponentParseFunc> StringToParseMap;

	static std::unique_ptr<GameObjectConfig> Parse(const pugi::xml_node& gameObjectNode);
};
}
