#pragma once

#include <glm/vec2.hpp>

namespace pugi { class xml_node; }

namespace diamond_engine {
class Vector2Parser {
public:
	static glm::vec2 Parse(const pugi::xml_node& node);
};
}
