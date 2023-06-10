#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "Transform.h"

namespace diamond_engine {
	/* static */ const std::string Transform::kModelUniformLocation = "model";

	void Transform::Translate(const glm::vec3& displacement) {
		m_localToWorld = glm::translate(m_localToWorld, displacement);
		SetDirty();
	}

	void Transform::Rotate(GLfloat degrees, const glm::vec3& axis) {
		m_localToWorld = glm::rotate(m_localToWorld, glm::radians(degrees), axis);
		SetDirty();
	}

	void Transform::SetLocalRotation(GLfloat degrees, const glm::vec3& axis) {
		m_localRotation = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
		SetDirty();
	}

	void Transform::SetLocalScale(const glm::vec3& scale) {
		m_localScale = glm::scale(glm::mat4(1.0f), scale);
		SetDirty();
	}

	void Transform::BindToContext() {
		//m_modelUniformLocation = glGetUniformLocation(m_gameObject->GetShaderProgram()->GetObject(), kModelUniformLocation.c_str());
	}

	void Transform::Update(GLfloat deltaTime) {
		/*m_modelMatrix = m_localToWorld * m_localScale * m_localRotation;

		glUniformMatrix4fv(
			m_modelUniformLocation,
			1,
			GL_FALSE,
			glm::value_ptr(m_modelMatrix)
		);*/

		UnsetDirty();
	}

	void Transform::SetDirty() {
		m_dirtyFlag |= DirtyFlag::SET;
	}

	void Transform::UnsetDirty() {
		m_dirtyFlag = DirtyFlag::UNSET;
	}
}