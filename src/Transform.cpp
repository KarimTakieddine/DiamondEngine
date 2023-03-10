#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
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

	void Transform::SetLocalScale(const glm::vec3& scale) {
		m_localScale = glm::scale(glm::mat4(1.0f), scale);
	}

	void Transform::BindToContext() {
		m_modelUniformLocation = glGetUniformLocation(m_gameObject->GetShaderProgram()->GetObject(), kModelUniformLocation.c_str());
	}

	void Transform::Update(GLfloat deltaTime) {
		const glm::mat4 modelMatrix = m_localToWorld * m_localScale * m_localRotation;

		glUniformMatrix4fv(
			m_modelUniformLocation,
			1,
			GL_FALSE,
			glm::value_ptr(modelMatrix)
		);
	}
}