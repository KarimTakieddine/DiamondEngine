#include <glm/gtc/type_ptr.hpp>

#include "rendering/Camera.h"
#include "gameobject/GameObject.h"
#include "rendering/Transform.h"

/*
	From the OpenGL FAQ at:

	https://www.opengl.org/archives/resources/faq/technical/viewing.htm

		8.010 How does the camera work in OpenGL?

		As far as OpenGL is concerned, there is no camera.
		More specifically, the camera is always located at
		the eye space coordinate (0., 0., 0.).

		To give the appearance of moving the camera,
		your OpenGL application must move the scene with
		the inverse of the camera transformation.
*/

namespace diamond_engine {
	void Camera::SetProjectionFrustum(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane) {
		m_projection = glm::perspective(
			glm::radians(fieldOfView),
			aspectRatio,
			nearPlane,
			farPlane
		);

		SetFieldOfView(fieldOfView);
		SetAspectRatio(aspectRatio);
		SetNearPlane(nearPlane);
		SetFarPlane(farPlane);
	}

	void Camera::SetFocusTarget(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
		m_view = glm::lookAtRH(
			eye,
			target,
			up
		);
	}

	void Camera::SetFieldOfView(GLfloat fieldOfView)
	{
		if (fieldOfView == m_fieldOfView)
		{
			return;
		}

		m_fieldOfView = fieldOfView;

		SetProjectionFrustum(fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
	}

	void Camera::SetAspectRatio(GLfloat aspectRatio)
	{
		if (aspectRatio == m_aspectRatio)
		{
			return;
		}

		m_aspectRatio = aspectRatio;

		SetProjectionFrustum(m_fieldOfView, aspectRatio, m_nearPlane, m_farPlane);
	}

	void Camera::SetNearPlane(GLfloat nearPlane)
	{
		if (nearPlane == m_nearPlane)
		{
			return;
		}

		m_nearPlane = nearPlane;

		SetProjectionFrustum(m_fieldOfView, m_aspectRatio, nearPlane, m_farPlane);
	}

	void Camera::SetFarPlane(GLfloat farPlane)
	{
		if (farPlane == m_farPlane)
		{
			return;
		}

		m_farPlane = farPlane;

		SetProjectionFrustum(m_fieldOfView, m_aspectRatio, m_nearPlane, farPlane);
	}

	const glm::mat4& Camera::GetView() const {
		return m_view;
	}

	const glm::mat4& Camera::GetProjection() const {
		return m_projection;
	}

	const Transform& Camera::GetTransform() const {
		return m_transform;
	}
}