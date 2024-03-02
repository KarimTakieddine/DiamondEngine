#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "Transform.h"

namespace diamond_engine {
	/* static */ const std::string Transform::kModelUniformLocation = "model";

	void Transform::Translate(const glm::vec3& displacement) {
		m_localToWorld = glm::translate(m_localToWorld, displacement);
	}

	void Transform::Translate(const glm::vec2& displacement)
	{
		Translate(glm::vec3{ displacement.x, displacement.y, 0.0f });
	}

	void Transform::Rotate(GLfloat degrees, const glm::vec3& axis) {
		m_localToWorld = glm::rotate(m_localToWorld, glm::radians(degrees), axis);
	}

	void Transform::SetLocalEulerAngles(const glm::vec3& localEulerAngles) {
		m_localRotation = glm::eulerAngleXYZ(
			glm::radians(localEulerAngles.x),
			glm::radians(localEulerAngles.y),
			glm::radians(localEulerAngles.z));
	}

	void Transform::SetLocalScale(const glm::vec3& scale) {
		m_localScale = glm::scale(glm::mat4(1.0f), scale);
	}

	void Transform::Scale(const glm::vec3& scale) {
		m_localToWorld = glm::scale(m_localToWorld, scale);
	}

	void Transform::SetPosition(const glm::vec3& position) {
		glm::vec4& currentPosition = m_localToWorld[3];

		currentPosition.x = position.x;
		currentPosition.y = position.y;
		currentPosition.z = position.z;
	}

	glm::vec3 Transform::getPosition() const
	{
		return glm::xyz(m_localToWorld * glm::vec4(m_position, 1.0f));
	}

	void diamond_engine::Transform::computeModelMatrix()
	{
		m_model = m_localToWorld * m_localRotation * m_localScale;
	}

	const glm::mat4& diamond_engine::Transform::getLocalToWorld() const
	{
		return m_localToWorld;
	}

	const glm::mat4& diamond_engine::Transform::getLocalRotation() const
	{
		return m_localRotation;
	}

	const glm::mat4& diamond_engine::Transform::getLocalScale() const
	{
		return m_localScale;
	}

	const glm::mat4& diamond_engine::Transform::getModel() const
	{
		return m_model;
	}
}