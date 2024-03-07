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
		{ "BUTTON_7", Button::BUTTON_7 }
	};

	ControllerConfig ControllerConfigParser::Parse(const pugi::xml_node& controllerConfigNode)
	{
		ControllerConfig result;

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

		return result;
	}
}