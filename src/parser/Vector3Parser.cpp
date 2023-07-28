#include <stdexcept>

#include <pugixml.hpp>

#include "parser/Vector3Parser.h"

namespace diamond_engine {
	/* static */ glm::vec3 Vector3Parser::Parse(const pugi::xml_node& node) {
		glm::vec3 result{ 0.0f, 0.0f, 0.0f };

		pugi::xml_attribute xAttribute = node.attribute("x");
		pugi::xml_attribute rAttribute = node.attribute("r");

		if (xAttribute) {
			if (rAttribute) {
				throw std::runtime_error("Cannot set both \"x\" and \"r\" attributes on Vector3 node");
			}

			result.x = xAttribute.as_float();
		}

		if (rAttribute) {
			if (xAttribute) {
				throw std::runtime_error("Cannot set both \"r\" and \"x\" attributes on Vector3 node");
			}

			result.r = rAttribute.as_float();
		}

		pugi::xml_attribute yAttribute = node.attribute("y");
		pugi::xml_attribute gAttribute = node.attribute("g");

		if (yAttribute) {
			if (gAttribute) {
				throw std::runtime_error("Cannot set both \"y\" and \"g\" attributes on Vector3 node");
			}

			result.y = yAttribute.as_float();
		}

		if (gAttribute) {
			if (yAttribute) {
				throw std::runtime_error("Cannot set both \"g\" and \"y\" attributes on Vector3 node");
			}

			result.g = gAttribute.as_float();
		}

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