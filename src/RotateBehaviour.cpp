#include "GameObject.h"
#include "RotateBehaviour.h"

namespace diamond_engine {
	void RotateBehaviour::Update(GLfloat deltaTime) {
		Transform& gameObjectTransform = m_gameObject->GetRenderableObject()->transform;

		gameObjectTransform.Rotate(m_xAnglePerSecond * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
		gameObjectTransform.Rotate(m_yAnglePerSecond * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		gameObjectTransform.Rotate(m_zAnglePerSecond * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void RotateBehaviour::SetXAnglePerSecond(GLfloat xAnglePerSecond) {
		m_xAnglePerSecond = xAnglePerSecond;
	}

	void RotateBehaviour::SetYAnglePerSecond(GLfloat yAnglePerSecond) {
		m_yAnglePerSecond = yAnglePerSecond;
	}

	void RotateBehaviour::SetZAnglePerSecond(GLfloat zAnglePerSecond) {
		m_zAnglePerSecond = zAnglePerSecond;
	}
}