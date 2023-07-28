#pragma once

#include "Transform.h"

namespace diamond_engine {
class Camera {
public:
	void SetProjectionFrustum(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

	void SetFocusTarget(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);

	void SetFieldOfView(GLfloat fieldOfView);

	void SetAspectRatio(GLfloat aspectRatio);

	void SetNearPlane(GLfloat nearPlane);

	void SetFarPlane(GLfloat farPlane);

	const glm::mat4& GetView() const;

	const glm::mat4& GetProjection() const;

	const Transform& GetTransform() const;

private:
	Transform m_transform{ };
	glm::mat4 m_projection{ 1.0f };
	glm::mat4 m_view{ 1.0f };
	glm::mat4 m_viewTransform{ 1.0f };
	GLfloat m_fieldOfView{ 1.0f };
	GLfloat m_aspectRatio{ 1.0f };
	GLfloat m_nearPlane{ 1.0f };
	GLfloat m_farPlane{ 1.0f };
};
}
