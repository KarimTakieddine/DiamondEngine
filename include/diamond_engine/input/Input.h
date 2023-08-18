#pragma once

#include "Keyboard.h"

class GLFWwindow;

namespace diamond_engine {
namespace input {
class StateMonitor {
public:
	static StateMonitor& GetInstance();

	void MonitorStates(GLFWwindow* window);

	void RegisterKeyboardKey(const std::string& name, GLFWKeyCode code);

	bool IsKeyPressed(const std::string& name) const;

	bool IsKeyDown(const std::string& name) const;

	bool IsKeyReleased(const std::string& name) const;

private:
	Keyboard m_keyboard;
};

bool IsKeyPressed(const std::string& keyName);

bool IsKeyDown(const std::string& keyName);

bool IsKeyReleased(const std::string& keyName);
}
}
