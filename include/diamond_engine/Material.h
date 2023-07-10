#pragma once

#include <glm/vec3.hpp>

namespace diamond_engine {
class Material {
public:
	const glm::vec3& GetColor() const;

	void SetColor(const glm::vec3& color);

private:
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
};
}
