#include "Material.h"

namespace diamond_engine {
	const glm::vec3& Material::GetColor() const {
		return m_color;
	}

	void Material::SetColor(const glm::vec3& color) {
		m_color = color;
	}
}