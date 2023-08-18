#pragma once

#include "KeyboardConfig.h"
#include "WindowConfig.h"

namespace diamond_engine {
class EngineConfig {
public:
	EngineConfig() = default;

	EngineConfig(const KeyboardConfig& keyboardConfig, const WindowConfig& windowConfig);

	void SetWindowConfig(const WindowConfig& windowConfig);

	const WindowConfig& GetWindowConfig() const;

	void SetKeyboardConfig(const KeyboardConfig& keyboardConfig);

	const KeyboardConfig& GetKeyboardConfig() const;

private:
	KeyboardConfig m_keyboardConfig;
	WindowConfig m_windowConfig;
};
}
