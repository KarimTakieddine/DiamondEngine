#pragma once

#include "Transform.h"

namespace diamond_engine {
class Camera {
public:
	static const std::string kProjectionUniformLocation;
	static const std::string kViewUniformLocation;

	void Update();

	void SetProjectionFrustum(GLfloat height, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

	void SetFocusTarget(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);

	void SetHeight(GLfloat height);

	void SetAspectRatio(GLfloat aspectRatio);

	void SetNearPlane(GLfloat nearPlane);

	void SetFarPlane(GLfloat farPlane);

	const glm::mat4& GetViewTransform() const;

	const glm::mat4& GetProjection() const;

private:
	Transform m_transform;
	glm::mat4 m_projection{ 1.0f };
	glm::mat4 m_view{ 1.0f };
	glm::mat4 m_viewTransform{ 1.0f };
	GLfloat m_height{ 1.0f };
	GLfloat m_aspectRatio{ 1.0f };
	GLfloat m_nearPlane{ 1.0f };
	GLfloat m_farPlane{ 1.0f };
};
}
