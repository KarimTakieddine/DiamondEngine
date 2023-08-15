#include <pugixml.hpp>

#include "Vector2Parser.h"

namespace diamond_engine {
/* static */ glm::vec2 Vector2Parser::Parse(const pugi::xml_node& node) {
	glm::vec2 result{ 0.0f, 0.0f };

	pugi::xml_attribute xAttribute = node.attribute("x");
	if (xAttribute) {
		result.x = xAttribute.as_float();
	}

	pugi::xml_attribute yAttribute = node.attribute("y");
	if (yAttribute) {
		result.y = yAttribute.as_float();
	}

	return result;
}
}