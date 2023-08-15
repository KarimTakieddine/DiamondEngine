#include "Material.h"

namespace diamond_engine {
	const glm::vec3& Material::GetColor() const {
		return m_color;
	}

	void Material::SetColor(const glm::vec3& color) {
		m_color = color;
	}

	const glm::vec2& Material::GetTextureOffset() const {
		return m_textureOffset;
	}

	void Material::SetTextureOffset(const glm::vec2 textureOffset) {
		m_textureOffset = textureOffset;
	}

	GLint Material::GetTexture() const {
		return m_texture;
	}

	void Material::SetTexture(GLint texture) {
		m_texture = texture;
	}
}