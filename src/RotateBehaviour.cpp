#include "GameObject.h"
#include "RotateBehaviour.h"

namespace diamond_engine {
	void RotateBehaviour::Update(GLfloat deltaTime) {
		m_xAngle += deltaTime * 90.0f;
		m_yAngle += deltaTime * 90.0f;

		m_gameObject->GetRenderableObject()->transform.SetLocalRotation(m_xAngle, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}