#include "EngineConfig.h"

namespace diamond_engine {
	EngineConfig::EngineConfig(const KeyboardConfig& keyboardConfig, const WindowConfig& windowConfig) : m_keyboardConfig(keyboardConfig), m_windowConfig(windowConfig) { }

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
}