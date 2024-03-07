#pragma once

#include "ControllerConfig.h"
#include "KeyboardConfig.h"
#include "WindowConfig.h"

namespace diamond_engine {
class EngineConfig {
public:
	EngineConfig() = default;

	EngineConfig(
		const KeyboardConfig& keyboardConfig,
		const ControllerConfig& controllerConfig,
		const WindowConfig& windowConfig);

	void SetWindowConfig(const WindowConfig& windowConfig);

	const WindowConfig& GetWindowConfig() const;

	void SetKeyboardConfig(const KeyboardConfig& keyboardConfig);

	const KeyboardConfig& GetKeyboardConfig() const;

	void setControllerConfig(const ControllerConfig& controllerConfig);

	const ControllerConfig& getControllerConfig() const;

private:
	KeyboardConfig m_keyboardConfig;
	ControllerConfig m_controllerConfig;
	WindowConfig m_windowConfig;
};
}
