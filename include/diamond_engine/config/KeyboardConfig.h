#pragma once

#include "Keyboard.h"

namespace diamond_engine {
struct KeyConfig {
	std::string name;
	input::GLFWKeyCode code;
};

class KeyboardConfig {
public:
	const std::vector<KeyConfig>& GetKeyConfigs() const;

	void AddKeyConfig(const KeyConfig& keyConfig);

private:
	std::vector<KeyConfig> m_keyConfigs;
};
}
