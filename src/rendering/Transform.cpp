#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

namespace diamond_engine {
	/* static */ const std::string Transform::kModelUniformLocation = "model";

	void Transform::Translate(const glm::vec3& displacement) {
		m_translation = glm::translate(m_translation, displacement);
	}

	void Transform::Rotate(GLfloat degrees, const glm::vec3& axis) {
		m_rotation = glm::rotate(m_rotation, glm::radians(degrees), axis);
	}

	void Transform::SetEulerAngles(const glm::vec3& localEulerAngles) {
		m_rotation = glm::eulerAngleXYZ(
			glm::radians(localEulerAngles.x),
			glm::radians(localEulerAngles.y),
			glm::radians(localEulerAngles.z));
	}

	void Transform::SetScale(const glm::vec3& scale) {
		m_scale = glm::scale(glm::mat4(1.0f), scale);
	}

	void Transform::SetPosition(const glm::vec3& position) {
		glm::vec4& currentPosition = m_translation[3];

		currentPosition.x = position.x;
		currentPosition.y = position.y;
		currentPosition.z = position.z;
	}
}