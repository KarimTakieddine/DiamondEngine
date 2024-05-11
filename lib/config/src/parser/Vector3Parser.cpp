#include <stdexcept>

#include <pugixml.hpp>

#include "Vector2Parser.h"
#include "Vector3Parser.h"

namespace diamond_engine {
	/* static */ glm::vec3 Vector3Parser::Parse(const pugi::xml_node& node) {
		glm::vec3 result{ Vector2Parser::Parse(node), 0.0f };

		pugi::xml_attribute zAttribute = node.attribute("z");
		pugi::xml_attribute bAttribute = node.attribute("b");

		if (zAttribute) {
			if (bAttribute) {
				throw std::runtime_error("Cannot set both \"z\" and \"b\" attributes on Vector3 node");
			}

			result.z = zAttribute.as_float();
		}

		if (bAttribute) {
			if (zAttribute) {
				throw std::runtime_error("Cannot set both \"b\" and \"z\" attributes on Vector3 node");
			}

			result.b = bAttribute.as_float();
		}

		return result;
	}
}