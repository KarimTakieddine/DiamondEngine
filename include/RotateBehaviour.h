#pragma once

#include "Behaviour.h"

namespace diamond_engine {
class RotateBehaviour : public Behaviour {
public:
	~RotateBehaviour() override = default;

	void Update(GLfloat deltaTime) override;

	void SetXAnglePerSecond(GLfloat xAnglePerSecond);

	void SetYAnglePerSecond(GLfloat yAnglePerSecond);

	void SetZAnglePerSecond(GLfloat zAnglePerSecond);

private:
	GLfloat m_xAnglePerSecond{ 0.0f };
	GLfloat m_yAnglePerSecond{ 0.0f };
	GLfloat m_zAnglePerSecond{ 0.0f };
};
}
