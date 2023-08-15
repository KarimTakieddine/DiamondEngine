#pragma once

#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace diamond_engine {
class Material {
public:
	const glm::vec3& GetColor() const;

	void SetColor(const glm::vec3& color);

	const glm::vec2& GetTextureOffset() const;

	void SetTextureOffset(const glm::vec2 textureOffset);

	GLint GetTexture() const;

	void SetTexture(GLint texture);

private:
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
	glm::vec2 m_textureOffset{ 0.0f, 0.0f };
	GLint m_texture{ 0 };
};
}
