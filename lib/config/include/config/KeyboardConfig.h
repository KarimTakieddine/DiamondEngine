#pragma once

#include <string>

namespace diamond_engine {
namespace input
{
	enum class GLFWKeyCode : int {
		KEYCODE_SPACE,
		KEYCODE_LEFT,
		KEYCODE_RIGHT,
		KEYCODE_UP,
		KEYCODE_DOWN,
		KEYCODE_ESCAPE
	};
}

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
