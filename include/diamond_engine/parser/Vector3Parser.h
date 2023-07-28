#pragma once

#include <glm/vec3.hpp>

namespace pugi { class xml_node; }

namespace diamond_engine {
class Vector3Parser {
public:
	static glm::vec3 Parse(const pugi::xml_node& node);
};
}
