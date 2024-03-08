#pragma once

#include <string>

#include <GL/glew.h>

#include <glm/vec2.hpp>

namespace diamond_engine {
class GameObject;

class Behaviour {
public:
	virtual ~Behaviour() = default;

	void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }

	virtual void Update(GLfloat deltaTime) = 0;
	virtual void OnCollisionEnter(const glm::vec2& resolutionAxis, const std::string& name) { }
	virtual void OnCollisionExit(const std::string& name) { }

protected:
	GameObject* m_gameObject{ nullptr };
};
}
