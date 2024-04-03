#include "MaterialComponentConfig.h"

namespace diamond_engine
{
	const char* MaterialComponentConfig::getName() const
	{
		return "Material";
	}

	const std::string& MaterialComponentConfig::getTextureName() const
	{
		return m_textureName;
	}

	void MaterialComponentConfig::setTextureName(const std::string& textureName)
	{
		m_textureName = textureName;
	}

	const glm::vec3& MaterialComponentConfig::getColor() const
	{
		return m_color;
	}

	void MaterialComponentConfig::setColor(const glm::vec3& color)
	{
		m_color = color;
	}

	const glm::vec2& MaterialComponentConfig::getTextureOffset() const
	{
		return m_textureOffset;
	}

	void MaterialComponentConfig::setTextureOffset(const glm::vec2& textureOffset)
	{
		m_textureOffset = textureOffset;
	}
}