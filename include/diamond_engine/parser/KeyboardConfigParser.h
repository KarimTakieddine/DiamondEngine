#pragma once

#include "KeyboardConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class KeyboardConfigParser {
public:
	static const std::unordered_map<std::string, input::GLFWKeyCode> kStringToKeyCodeMap;

	static KeyboardConfig Parse(const pugi::xml_node& keyboardConfigNode);
};
}
