#pragma once

#include <string>
#include <type_traits>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace diamond_engine {
class Transform {
public:
	static const std::string kModelUniformLocation;

	void Translate(const glm::vec3& displacement);

	void Rotate(GLfloat degrees, const glm::vec3& axis);

	void SetEulerAngles(const glm::vec3& localEulerAngles);

	void SetScale(const glm::vec3& scale);

	const glm::mat4& GetTranslation() const {
		return m_translation;
	}

	const glm::mat4& GetRotation() const {
		return m_rotation;
	}

	const glm::mat4& GetScale() const {
		return m_scale;
	}

	void SetPosition(const glm::vec3& position);

private:
	glm::mat4 m_translation{ 1.0f };
	glm::mat4 m_rotation{ 1.0f };
	glm::mat4 m_scale{ 1.0f };
};
}
