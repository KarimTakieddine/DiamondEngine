#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace diamond_engine {
struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textureCoordinates;
};
}
