#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

namespace diamond_engine {
	/* static */ const std::string Transform::kModelUniformLocation = "model";

	void Transform::Translate(const glm::vec3& displacement) {
		m_localToWorld = glm::translate(m_localToWorld, displacement);
	}

	void Transform::Rotate(GLfloat degrees, const glm::vec3& axis) {
		m_localToWorld = glm::rotate(m_localToWorld, glm::radians(degrees), axis);
	}

	void Transform::SetLocalRotation(GLfloat degrees, const glm::vec3& axis) {
		m_localRotation = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
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

	void Transform::SetPosition(const glm::vec3& position) {
		glm::vec4& currentPosition = m_localToWorld[3];

		currentPosition.x = position.x;
		currentPosition.y = position.y;
		currentPosition.z = position.z;
	}
}