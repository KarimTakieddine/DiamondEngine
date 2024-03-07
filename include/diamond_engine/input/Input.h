#pragma once

#include <memory>

#include "Controller.h"
#include "Keyboard.h"

struct GLFWwindow;

namespace diamond_engine {
namespace input {
class StateMonitor {
public:
	static constexpr unsigned long kMaxControllerCount = 4;

	static StateMonitor& GetInstance();

	void MonitorStates(GLFWwindow* window);

	void RegisterKeyboardKey(const std::string& name, GLFWKeyCode code);

	void RegisterControllerButton(const std::string& name, Button button);

	void RegisterControllerJoystick(const std::string& name, Joystick joystick);

	bool IsKeyPressed(const std::string& name) const;

	bool IsKeyDown(const std::string& name) const;

	bool IsKeyReleased(const std::string& name) const;

	bool IsButtonPressed(const std::string& name) const;

	bool IsButtonDown(const std::string& name) const;

	bool IsButtonReleased(const std::string& name) const;

	glm::vec2 getJoystickInput(const std::string& name) const;

	void setJoystickDeadzone(float joystickDeadzone);

private:
	StateMonitor();

	Keyboard m_keyboard;
	std::vector<std::unique_ptr<Controller>> m_controllers;
};

bool IsKeyPressed(const std::string& keyName);

bool IsKeyDown(const std::string& keyName);

bool IsKeyReleased(const std::string& keyName);
}
}
