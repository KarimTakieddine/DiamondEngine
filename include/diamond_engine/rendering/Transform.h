#pragma once

#include <string>
#include <type_traits>

#include <GL/glew.h>

#define GL_FORCE_SWIZZLE
#include <glm/glm.hpp>

namespace diamond_engine {
class Transform {
public:
	static const std::string kModelUniformLocation;

	void Translate(const glm::vec3& displacement);

	void Translate(const glm::vec2& displacement);

	void Rotate(GLfloat degrees, const glm::vec3& axis);

	void SetLocalEulerAngles(const glm::vec3& localEulerAngles);

	void SetLocalScale(const glm::vec3& scale);

	void Scale(const glm::vec3& scale);

	void SetPosition(const glm::vec3& position);

	glm::vec3 getPosition() const;

	void computeModelMatrix();

	const glm::mat4& getLocalToWorld() const;

	const glm::mat4& getLocalRotation() const;

	const glm::mat4& getLocalScale() const;

	const glm::mat4& getModel() const;

private:
	glm::mat4 m_localToWorld{ 1.0f };
	glm::mat4 m_localRotation{ 1.0f };
	glm::mat4 m_localScale{ 1.0f };
	glm::mat4 m_model{ 1.0f };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
};
}
