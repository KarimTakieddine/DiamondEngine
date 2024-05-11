#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace diamond_engine
{
	struct RenderMaterial
	{
		glm::vec3 color{ 1.0f, 1.0f, 1.0f };
		glm::vec2 textureOffset{ 0.0f, 0.0f };
	};
}
