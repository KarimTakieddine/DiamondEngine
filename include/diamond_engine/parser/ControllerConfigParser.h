#pragma once

#include <unordered_map>

#include "ControllerConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class ControllerConfigParser
	{
	public:
		static const std::unordered_map<std::string, Button> kStringToButtonMap;

		static const std::unordered_map<std::string, Joystick> kStringToJoystickMap;

		static ControllerConfig Parse(const pugi::xml_node& controllerConfigNode);
	};
}
