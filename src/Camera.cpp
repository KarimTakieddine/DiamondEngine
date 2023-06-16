#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

namespace diamond_engine {
	/* static */ const std::string Camera::kProjectionUniformLocation = "projection";

	/* static */ const std::string Camera::kViewUniformLocation = "view";

	void Camera::Update() {
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

		m_viewTransform = m_view * glm::inverse(m_transform.GetLocalToWorldMatrix());

		//glUniformMatrix4fv(m_projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(m_projection));
		//glUniformMatrix4fv(m_viewUniformLocation, 1, GL_FALSE, glm::value_ptr(viewTransform));
	}

	void Camera::SetProjectionFrustum(GLfloat height, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane) {
		GLfloat fieldOfView = 2 * atan(height * 0.5f);

		m_projection = glm::perspective(
			fieldOfView,
			aspectRatio,
			nearPlane,
			farPlane
		);

		SetHeight(height);
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

	void Camera::SetHeight(GLfloat height)
	{
		if (height == m_height)
		{
			return;
		}

		m_height = height;

		SetProjectionFrustum(height, m_aspectRatio, m_nearPlane, m_farPlane);
	}

	void Camera::SetAspectRatio(GLfloat aspectRatio)
	{
		if (aspectRatio == m_aspectRatio)
		{
			return;
		}

		m_aspectRatio = aspectRatio;

		SetProjectionFrustum(m_height, aspectRatio, m_nearPlane, m_farPlane);
	}

	void Camera::SetNearPlane(GLfloat nearPlane)
	{
		if (nearPlane == m_nearPlane)
		{
			return;
		}

		m_nearPlane = nearPlane;

		SetProjectionFrustum(m_height, m_aspectRatio, nearPlane, m_farPlane);
	}

	void Camera::SetFarPlane(GLfloat farPlane)
	{
		if (farPlane == m_farPlane)
		{
			return;
		}

		m_farPlane = farPlane;

		SetProjectionFrustum(m_height, m_aspectRatio, m_nearPlane, farPlane);
	}

	const glm::mat4& Camera::GetViewTransform() const {
		return m_viewTransform;
	}

	const glm::mat4& Camera::GetProjection() const {
		return m_projection;
	}
}