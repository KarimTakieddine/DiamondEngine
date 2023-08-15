#include "MaterialConfig.h"

namespace diamond_engine {
	const std::string& MaterialConfig::GetTextureName() const {
		return m_textureName;
	}

	void MaterialConfig::SetTextureName(const std::string& textureName) {
		m_textureName = textureName;
	}
	
	const glm::vec3& MaterialConfig::GetColor() const {
		return m_color;
	}

	void MaterialConfig::SetColor(const glm::vec3& color) {
		m_color = color;
	}
	
	const glm::vec2& MaterialConfig::GetTextureOffset() const {
		return m_textureOffset;
	}
	
	void MaterialConfig::SetTextureOffset(const glm::vec2& textureOffset) {
		m_textureOffset = textureOffset;
	}
}