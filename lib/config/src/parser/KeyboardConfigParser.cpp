#include <stdexcept>

#include <pugixml.hpp>

#include "KeyboardConfigParser.h"

namespace diamond_engine {
	/* static */ const std::unordered_map<std::string, KeyCode> KeyboardConfigParser::kStringToKeyCodeMap = {
		{ "SPACE",	KeyCode::KEYCODE_SPACE },
		{ "ESCAPE", KeyCode::KEYCODE_ESCAPE },
		{ "UP",		KeyCode::KEYCODE_UP },
		{ "DOWN",	KeyCode::KEYCODE_DOWN },
		{ "LEFT",	KeyCode::KEYCODE_LEFT },
		{ "RIGHT",	KeyCode::KEYCODE_RIGHT }
	};


	/* static */ KeyboardConfig KeyboardConfigParser::Parse(const pugi::xml_node& keyboardConfigNode) {
		KeyboardConfig result;

		for (const auto& keyConfigNode : keyboardConfigNode.children("Key")) {
			pugi::xml_attribute nameAttribute = keyConfigNode.attribute("name");

			if (!nameAttribute) {
				throw std::runtime_error("Could not find \"name\" attribute on <Key/> config node");
			}

			const std::string name(nameAttribute.as_string());

			pugi::xml_attribute codeAttribute = keyConfigNode.attribute("code");

			if (!codeAttribute) {
				throw std::runtime_error("Could not find \"code\" attribute on <Key/> config node");
			}

			const std::string code(codeAttribute.as_string());

			auto keyCodeIt = kStringToKeyCodeMap.find(code);

			if (keyCodeIt == kStringToKeyCodeMap.cend()) {
				throw std::runtime_error("Invalid or unknown key code specified: " + code);
			}

			result.AddKeyConfig({ name, keyCodeIt->second });
		}

		return result;
	}
}