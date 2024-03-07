#pragma once

#include <vector>

#include "Controller.h"

namespace diamond_engine
{
	class ControllerConfig
	{
	public:
		struct ButtonConfig
		{
			std::string name;
			Button button;
		};
		
		const std::vector<ButtonConfig>& getButtonConfigs() const;
		void addButtonConfig(const ButtonConfig& buttonConfig);

	private:
		std::vector<ButtonConfig> m_buttonConfigs;
	};
}
