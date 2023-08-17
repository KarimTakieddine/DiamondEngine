#include "GameObject.h"
#include "RotateBehaviour.h"

namespace diamond_engine {
	void RotateBehaviour::Update(GLfloat deltaTime) {
		Transform& gameObjectTransform = m_gameObject->GetRenderableObject()->transform;

		m_xAngle += m_xAnglePerSecond * deltaTime;
		m_yAngle += m_yAnglePerSecond * deltaTime;
		m_zAngle += m_zAnglePerSecond * deltaTime;

		m_gameObject->GetRenderableObject()->transform.SetEulerAngles({ m_xAngle, m_yAngle, m_zAngle });
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