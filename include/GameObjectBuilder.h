#pragma once

#include <GL/glew.h>

namespace diamond_engine {
class GameObject;

class GameObjectBuilder {
public:
	void SetGameObject(GameObject* gameObject);

	virtual void SetCapacity(GLsizei capacity)	= 0;
	virtual void AddComponents()				= 0;

protected:
	GameObject* m_gameObject{ nullptr };
};
}
