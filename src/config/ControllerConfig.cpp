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
}