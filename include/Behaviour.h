#pragma once

#include <GL/glew.h>

namespace diamond_engine {
class GameObject;

class Behaviour {
public:
	virtual ~Behaviour() = default;

	void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }

	virtual void Update(GLfloat deltaTime) = 0;

protected:
	GameObject* m_gameObject{ nullptr };
};
}
