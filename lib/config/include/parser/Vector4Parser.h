#pragma once

#include <glm/vec4.hpp>

namespace pugi { class xml_node; }

namespace diamond_engine {
class Vector4Parser {
public:
	static glm::vec4 Parse(const pugi::xml_node& node);
};
}
