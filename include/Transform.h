#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Component.h"

namespace diamond_engine {
// TODO: Separate updatable components from Renderable components. Possibly use multiple inheritance here?
class Transform : public Component {
public:
	static const std::string kModelUniformLocation;

	void Translate(const glm::vec3& displacement);

	void Rotate(GLfloat degrees, const glm::vec3& axis);

	void SetLocalRotation(GLfloat degrees, const glm::vec3& axis);

	void SetLocalScale(const glm::vec3& scale);

	void BindToContext() override;

	void Render() override { }

	void Update(GLfloat deltaTime) override;

	const char* GetName() const override {
		return "Transform";
	}

	const glm::mat4& GetLocalToWorldMatrix() const {
		return m_localToWorld;
	}

protected:
	void OnSetGameObject() override { }

	void OnGameObjectAboutToBeUnset() override { }

private:
	glm::mat4 m_localToWorld{ 1.0f };
	glm::mat4 m_localRotation{ 1.0f };
	glm::mat4 m_localScale{ 1.0f };
	GLint m_modelUniformLocation{ -1 };
};
}
