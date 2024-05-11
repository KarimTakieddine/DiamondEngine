#include <stdexcept>

#include <pugixml.hpp>

#include "Vector3Parser.h"
#include "Vector4Parser.h"

namespace diamond_engine {
	/* static */ glm::vec4 Vector4Parser::Parse(const pugi::xml_node& node) {
		glm::vec4 result{ Vector3Parser::Parse(node), 1.0f };

		pugi::xml_attribute wAttribute = node.attribute("w");
		pugi::xml_attribute aAttribute = node.attribute("a");

		if (wAttribute) {
			if (aAttribute) {
				throw std::runtime_error("Cannot set both \"w\" and \"a\" attributes on Vector4 node");
			}

			result.w = wAttribute.as_float();
		}

		if (aAttribute) {
			if (wAttribute) {
				throw std::runtime_error("Cannot set both \"a\" and \"w\" attributes on Vector4 node");
			}

			result.a = aAttribute.as_float();
		}

		return result;
	}
}