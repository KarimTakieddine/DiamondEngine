#pragma once

#include "Behaviour.h"

namespace diamond_engine {
class RotateBehaviour : public Behaviour {
public:
	~RotateBehaviour() override = default;

	void Update(GLfloat deltaTime) override;

private:
	GLfloat m_xIncrement{ 0.0f };
	GLfloat m_xAngle{ 0.0f };
	GLfloat m_yIncrement{ 0.0f };
	GLfloat m_yAngle{ 0.0f };
	GLfloat m_zIncrement{ 0.0f };
	GLfloat m_zAngle{ 0.0f };
};
}
