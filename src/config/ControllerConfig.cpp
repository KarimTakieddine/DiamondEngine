#include "ControllerConfig.h"

namespace diamond_engine
{
	const std::vector<ControllerConfig::ButtonConfig>& ControllerConfig::getButtonConfigs() const
	{
		return m_buttonConfigs;
	}

	void ControllerConfig::addButtonConfig(const ButtonConfig& buttonConfig)
	{
		m_buttonConfigs.push_back(buttonConfig);
	}

	const std::vector<ControllerConfig::JoystickConfig>& ControllerConfig::getJoystickConfigs() const
	{
		return m_joystickConfigs;
	}

	void ControllerConfig::addJoystickConfig(const JoystickConfig& joystickConfig)
	{
		m_joystickConfigs.push_back(joystickConfig);
	}

	float ControllerConfig::getJoystickDeadzone() const
	{
		return m_joystickDeadzone;
	}

	void ControllerConfig::setJoystickDeadzone(float joystickDeadzone)
	{
		m_joystickDeadzone = joystickDeadzone;
	}
}