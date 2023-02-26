#pragma once

#include <GL/glew.h>

#include "GameObject.h"

namespace diamond_engine {
class GameObjectBuilder {
public:
	void SetGameObject(GameObject* gameObject);

	virtual void SetCapacity(GLsizei capacity)	= 0;
	virtual void AddComponents()				= 0;

	virtual ~GameObjectBuilder() = default;

protected:
	GameObject* m_gameObject{ nullptr };
};
}
