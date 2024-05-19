#include "EngineConfig.h"

namespace diamond_engine {
	EngineConfig::EngineConfig(
		const KeyboardConfig& keyboardConfig,
		const ControllerConfig& controllerConfig,
		const WindowConfig& windowConfig) :
		m_keyboardConfig(keyboardConfig),
		m_controllerConfig(controllerConfig),
		m_windowConfig(windowConfig),
		m_shadersDirectory(),
		m_texturesDirectory() { }

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

	void EngineConfig::setShadersDirectory(const std::string& shadersDirectory)
	{
		m_shadersDirectory = shadersDirectory;
	}

	const std::string& EngineConfig::getShadersDirectory() const
	{
		return m_shadersDirectory;
	}

	void EngineConfig::setTexturesDirectory(const std::string& texturesDirectory)
	{
		m_texturesDirectory = texturesDirectory;
	}

	const std::string& EngineConfig::getTexturesDirectory() const
	{
		return m_texturesDirectory;
	}

	void EngineConfig::setAudioDirectory(const std::string& audioDirectory)
	{
		m_audioDirectory = audioDirectory;
	}

	const std::string& EngineConfig::getAudioDirectory() const
	{
		return m_audioDirectory;
	}

	void EngineConfig::setSpriteSheetsDirectory(const std::string& spriteSheetsDirectory)
	{
		m_spriteSheetsDirectory = spriteSheetsDirectory;
	}

	const std::string& EngineConfig::getSpriteSheetsDirectory() const
	{
		return m_spriteSheetsDirectory;
	}
}