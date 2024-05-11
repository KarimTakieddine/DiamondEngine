#pragma once

#include <unordered_map>

#include "KeyboardConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class KeyboardConfigParser {
public:
	static const std::unordered_map<std::string, KeyCode> kStringToKeyCodeMap;

	static KeyboardConfig Parse(const pugi::xml_node& keyboardConfigNode);
};
}
