#include <glm/gtc/matrix_transform.hpp>

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

	void Transform::SetPosition(const glm::vec3& position) {
		glm::vec4& currentPosition = m_localToWorld[3];

		currentPosition.x = position.x;
		currentPosition.y = position.y;
		currentPosition.z = position.z;

		SetDirty();
	}

	void Transform::Update() {
		m_model = m_localToWorld * m_localScale * m_localRotation;
		UnsetDirty();
	}

	const glm::mat4& Transform::GetModelMatrix() const {
		return m_model;
	}

	void Transform::SetDirty() {
		m_dirtyFlag |= DirtyFlag::SET;
	}

	void Transform::UnsetDirty() {
		m_dirtyFlag = DirtyFlag::UNSET;
	}
}