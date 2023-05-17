#pragma once

#include <memory>

#include "Transform.h"

namespace diamond_engine {
class Camera : public Component {
public:
	static const std::string kProjectionUniformLocation;
	static const std::string kViewUniformLocation;

	Camera();

	void BindToContext() override;

	void Render() override { }

	void Update(GLfloat deltaTime) override;

	void SetProjectionFrustum(GLfloat height, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

	void SetFocusTarget(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);

	void SetHeight(GLfloat height);

	void SetAspectRatio(GLfloat aspectRatio);

	void SetNearPlane(GLfloat nearPlane);

	void SetFarPlane(GLfloat farPlane);

	Transform* GetTransform();

	virtual const char* GetName() const {
		return "Camera";
	}

protected:
	void OnSetGameObject() override { }

	void OnGameObjectAboutToBeUnset() override { }

private:
	std::unique_ptr<Transform> m_transform{ nullptr };
	glm::mat4 m_projection{ 1.0f };
	glm::mat4 m_view{ 1.0f };
	GLfloat m_height{ 1.0f };
	GLfloat m_aspectRatio{ 1.0f };
	GLfloat m_nearPlane{ 1.0f };
	GLfloat m_farPlane{ 1.0f };
	GLint m_projectionUniformLocation{ -1 };
	GLint m_viewUniformLocation{ -1 };
};
}
