#pragma once

#include "ShaderProgram.h"

namespace diamond_engine {
class GameObject;

class Component {
public:
	void SetGameObject(GameObject* gameObject) {
		if (gameObject == m_gameObject) {
			return;
		}

		m_gameObject = gameObject;
	}

	virtual void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)	= 0;
	virtual void OnAddedToScene()															= 0;
	virtual const char * GetName() const													= 0;

	virtual void Update(GLfloat deltaTime) { }

	virtual ~Component() = default;

	GameObject* m_gameObject{ nullptr };
};
}
