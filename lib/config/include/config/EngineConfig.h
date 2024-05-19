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

	void setShadersDirectory(const std::string& shadersDirectory);

	const std::string& getShadersDirectory() const;

	void setTexturesDirectory(const std::string& texturesDirectory);

	const std::string& getTexturesDirectory() const;

	void setAudioDirectory(const std::string& audioDirectory);

	const std::string& getAudioDirectory() const;

	void setSpriteSheetsDirectory(const std::string& spriteSheetsDirectory);

	const std::string& getSpriteSheetsDirectory() const;

private:
	KeyboardConfig m_keyboardConfig;
	ControllerConfig m_controllerConfig;
	WindowConfig m_windowConfig;
	std::string m_shadersDirectory;
	std::string m_texturesDirectory;
	std::string m_audioDirectory;
	std::string m_spriteSheetsDirectory;
};
}
