#include "EngineConfig.h"
#include "EngineConfig.h"
#include "EngineConfig.h"
#include "EngineConfig.h"

namespace diamond_engine {
	EngineConfig::EngineConfig(
		const KeyboardConfig& keyboardConfig,
		const ControllerConfig& controllerConfig,
		const WindowConfig& windowConfig) :
		m_keyboardConfig(keyboardConfig), m_controllerConfig(controllerConfig), m_windowConfig(windowConfig) { }

	void EngineConfig::SetWindowConfig(const WindowConfig& windowConfig) {
		m_windowConfig = windowConfig;
	}

	const WindowConfig& EngineConfig::GetWindowConfig() const {
		return m_windowConfig;
	}

	void EngineConfig::SetKeyboardConfig(const KeyboardConfig& keyboardConfig) {
		m_keyboardConfig = keyboardConfig;
	}

	const KeyboardConfig& EngineConfig::GetKeyboardConfig() const {
		return m_keyboardConfig;
	}

	void EngineConfig::setControllerConfig(const ControllerConfig& controllerConfig)
	{
		m_controllerConfig = controllerConfig;
	}

	const ControllerConfig& EngineConfig::getControllerConfig() const
	{
		return m_controllerConfig;
	}
}