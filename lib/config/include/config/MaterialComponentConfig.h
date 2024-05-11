#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "RenderComponentConfig.h"

namespace diamond_engine
{
	class MaterialComponentConfig: public RenderComponentConfig
	{
	public:
		~MaterialComponentConfig() final override = default;
		const char* getName() const final override;

		const std::string& getTextureName() const;
		void setTextureName(const std::string& textureName);

		const glm::vec3& getColor() const;
		void setColor(const glm::vec3& color);

		const glm::vec2& getTextureOffset() const;
		void setTextureOffset(const glm::vec2& textureOffset);

	private:
		std::string m_textureName{ "Unnamed" };
		glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
		glm::vec2 m_textureOffset{ 0.0f, 0.0f };
	};
}
