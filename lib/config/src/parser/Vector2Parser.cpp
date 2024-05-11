#include <stdexcept>

#include <pugixml.hpp>

#include "Vector2Parser.h"

namespace diamond_engine {
/* static */ glm::vec2 Vector2Parser::Parse(const pugi::xml_node& node) {
	glm::vec2 result{ 0.0f, 0.0f };

	pugi::xml_attribute xAttribute = node.attribute("x");
	pugi::xml_attribute rAttribute = node.attribute("r");

	if (xAttribute) {
		if (rAttribute) {
			throw std::runtime_error("Cannot set both \"x\" and \"r\" attributes on Vector2 node");
		}

		result.x = xAttribute.as_float();
	}

	if (rAttribute) {
		if (xAttribute) {
			throw std::runtime_error("Cannot set both \"r\" and \"x\" attributes on Vector2 node");
		}

		result.r = rAttribute.as_float();
	}

	pugi::xml_attribute yAttribute = node.attribute("y");
	pugi::xml_attribute gAttribute = node.attribute("g");

	if (yAttribute) {
		if (gAttribute) {
			throw std::runtime_error("Cannot set both \"y\" and \"g\" attributes on Vector2 node");
		}

		result.y = yAttribute.as_float();
	}

	if (gAttribute) {
		if (yAttribute) {
			throw std::runtime_error("Cannot set both \"g\" and \"y\" attributes on Vector2 node");
		}

		result.g = gAttribute.as_float();
	}

	return result;
}
}