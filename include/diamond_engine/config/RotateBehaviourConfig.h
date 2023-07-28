#pragma once

#include <GL/glew.h>

#include "BehaviourConfig.h"

namespace diamond_engine {
class RotateBehaviourConfig : public BehaviourConfig {
public:
	~RotateBehaviourConfig() override = default;

	const char* GetName() const override {
		return "RotateBehaviour";
	}

	void SetXAnglePerSecond(GLfloat xAnglePerSecond) {
		m_xAnglePerSecond = xAnglePerSecond;
	}

	GLfloat GetXAnglePerSecond() const {
		return m_xAnglePerSecond;
	}

	void SetYAnglePerSecond(GLfloat yAnglePerSecond) {
		m_yAnglePerSecond = yAnglePerSecond;
	}

	GLfloat GetYAnglePerSecond() const {
		return m_yAnglePerSecond;
	}

	void SetZAnglePerSecond(GLfloat zAnglePerSecond) {
		m_zAnglePerSecond = zAnglePerSecond;
	}

	GLfloat GetZAnglePerSecond() const {
		return m_zAnglePerSecond;
	}

private:
	GLfloat m_xAnglePerSecond{ 0.0f };
	GLfloat m_yAnglePerSecond{ 0.0f };
	GLfloat m_zAnglePerSecond{ 0.0f };
};
}
