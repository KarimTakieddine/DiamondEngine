#pragma once

#include <GL/glew.h>

namespace diamond_engine {
class GameObject;

class Component {
public:
	void SetGameObject(GameObject* gameObject) {
		if (gameObject == m_gameObject) {
			return;
		}

		m_gameObject = gameObject;

		OnSetGameObject();
	}
	void UnsetGameObject() {
		if (!m_gameObject) {
			return;
		}

		OnGameObjectAboutToBeUnset();

		m_gameObject = nullptr;
	}

	virtual void BindToContext()			= 0;
	virtual void Update(GLfloat deltaTime)	= 0;
	virtual void Render()					= 0;
	virtual const char * GetName() const	= 0;

	virtual ~Component() = default;

protected:
	virtual void OnSetGameObject()				= 0;
	virtual void OnGameObjectAboutToBeUnset()	= 0;

	GameObject* m_gameObject{ nullptr };
};
}
