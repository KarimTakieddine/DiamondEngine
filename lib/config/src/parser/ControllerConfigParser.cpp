#include <stdexcept>

#include <pugixml.hpp>

#include "ControllerConfigParser.h"

namespace diamond_engine
{
	/* static */ const std::unordered_map<std::string, Button> ControllerConfigParser::kStringToButtonMap =
	{
		{ "BUTTON_0", Button::BUTTON_0 },
		{ "BUTTON_1", Button::BUTTON_1 },
		{ "BUTTON_2", Button::BUTTON_2 },
		{ "BUTTON_3", Button::BUTTON_3 },
		{ "BUTTON_4", Button::BUTTON_4 },
		{ "BUTTON_5", Button::BUTTON_5 },
		{ "BUTTON_6", Button::BUTTON_6 },
		{ "BUTTON_7", Button::BUTTON_7 },
		{ "BUTTON_8", Button::BUTTON_8 },
		{ "BUTTON_9", Button::BUTTON_9 },
		{ "BUTTON_10", Button::BUTTON_10 },
		{ "BUTTON_11", Button::BUTTON_11 }
	};

	/* static */ const std::unordered_map<std::string, Joystick> ControllerConfigParser::kStringToJoystickMap =
	{
		{ "LEFT", Joystick::LEFT },
		{ "RIGHT", Joystick::RIGHT }
	};

	ControllerConfig ControllerConfigParser::Parse(const pugi::xml_node& controllerConfigNode)
	{
		ControllerConfig result;

		pugi::xml_attribute joystickDeadzoneAttribute = controllerConfigNode.attribute("joystickDeadzone");

		if (joystickDeadzoneAttribute)
		{
			result.setJoystickDeadzone(joystickDeadzoneAttribute.as_float(0.0f));
		}

		pugi::xml_node buttonsNode = controllerConfigNode.child("Buttons");

		if (buttonsNode)
		{
			for (const auto& buttonNode : buttonsNode.children("Button"))
			{
				pugi::xml_attribute nameAttribute = buttonNode.attribute("name");

				if (!nameAttribute)
				{
					throw std::runtime_error("<Button/> node must have a value for the \"name\" attribute");
				}

				const std::string name(nameAttribute.as_string());

				pugi::xml_attribute buttonAttribute = buttonNode.attribute("button");

				if (!buttonAttribute)
				{
					throw std::runtime_error("<Button/> node must have a value for the \"button\" attribute");
				}

				const std::string button(buttonAttribute.as_string());

				auto buttonIt = kStringToButtonMap.find(button);

				if (buttonIt == kStringToButtonMap.cend())
				{
					throw std::runtime_error("Unrecognised button: " + button);
				}

				result.addButtonConfig({ name, buttonIt->second });
			}
		}

		pugi::xml_node joysticksNode = controllerConfigNode.child("Joysticks");

		if (joysticksNode)
		{
			for (const auto& joystickNode : joysticksNode.children("Joystick"))
			{
				pugi::xml_attribute nameAttribute = joystickNode.attribute("name");

				if (!nameAttribute)
				{
					throw std::runtime_error("<Joystick/> node must have a value for the \"name\" attribute");
				}

				std::string name(nameAttribute.as_string());

				pugi::xml_attribute joystickAttribute = joystickNode.attribute("joystick");

				if (!joystickAttribute)
				{
					throw std::runtime_error("<Joystick/> node must have a value for the \"joystick\" attribute");
				}

				const std::string joystick(joystickAttribute.as_string());

				auto joystickIt = kStringToJoystickMap.find(joystick);

				if (joystickIt == kStringToJoystickMap.cend())
				{
					throw std::runtime_error("Unrecognised joystick: " + joystick);
				}

				result.addJoystickConfig({ name, joystickIt->second });
			}
		}

		return result;
	}
}