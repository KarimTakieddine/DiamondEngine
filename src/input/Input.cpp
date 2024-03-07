#include <algorithm>

#include "Input.h"

#ifdef WIN32
#include "XInputController.h"
#endif

namespace diamond_engine {
namespace input {
	/* static */ StateMonitor& StateMonitor::GetInstance() {
		static StateMonitor stateMonitorInstance;
		
		return stateMonitorInstance;
	}

	StateMonitor::StateMonitor() : m_keyboard(), m_controllers()
	{
		for (unsigned long i = 0; i < kMaxControllerCount; ++i)
		{
#ifdef WIN32
			std::unique_ptr<XInputController> controller = std::make_unique<XInputController>();
			m_controllers.push_back(std::move(controller));
#endif
		}
	}

	void StateMonitor::MonitorStates(GLFWwindow* window) {
		m_keyboard.UpdateKeyStates(window);

		for (unsigned long i = 0; i < kMaxControllerCount; ++i)
		{
			m_controllers[i]->refreshState(i);
		}
	}

	void StateMonitor::RegisterKeyboardKey(const std::string& name, GLFWKeyCode code) {
		m_keyboard.RegisterKey(name, code);
	}

	void StateMonitor::RegisterControllerButton(const std::string& name, Button button)
	{
		for (unsigned long i = 0; i < kMaxControllerCount; ++i)
		{
			m_controllers[i]->registerButton(name, button);
		}
	}

	void StateMonitor::RegisterControllerJoystick(const std::string& name, Joystick joystick)
	{
		for (unsigned long i = 0; i < kMaxControllerCount; ++i)
		{
			m_controllers[i]->registerJoystick(name, joystick);
		}
	}

	bool StateMonitor::IsKeyPressed(const std::string& name) const {
		return m_keyboard.IsKeyPressed(name);
	}

	bool StateMonitor::IsKeyDown(const std::string& name) const {
		return m_keyboard.IsKeyDown(name);
	}

	bool StateMonitor::IsKeyReleased(const std::string& name) const {
		return m_keyboard.IsKeyReleased(name);
	}

	bool StateMonitor::IsButtonPressed(const std::string& name) const
	{
		auto controllerIt = std::find_if(m_controllers.cbegin(), m_controllers.cend(), [](const auto& controller) { return controller->isConnected(); });

		if (controllerIt == m_controllers.cend())
		{
			return false;
		}

		return (*controllerIt)->isButtonPressed(name);
	}

	bool StateMonitor::IsButtonDown(const std::string& name) const
	{
		auto controllerIt = std::find_if(m_controllers.cbegin(), m_controllers.cend(), [](const auto& controller) { return controller->isConnected(); });

		if (controllerIt == m_controllers.cend())
		{
			return false;
		}

		return (*controllerIt)->isButtonDown(name);
	}

	bool StateMonitor::IsButtonReleased(const std::string& name) const
	{
		auto controllerIt = std::find_if(m_controllers.cbegin(), m_controllers.cend(), [](const auto& controller) { return controller->isConnected(); });

		if (controllerIt == m_controllers.cend())
		{
			return false;
		}

		return (*controllerIt)->isButtonReleased(name);
	}

	glm::vec2 StateMonitor::getJoystickInput(const std::string& name) const
	{
		auto controllerIt = std::find_if(m_controllers.cbegin(), m_controllers.cend(), [](const auto& controller) { return controller->isConnected(); });

		if (controllerIt == m_controllers.cend())
		{
			return { };
		}

		return (*controllerIt)->getJoystickInput(name);
	}

	void StateMonitor::setJoystickDeadzone(float joystickDeadzone)
	{
		for (unsigned long i = 0; i < kMaxControllerCount; ++i)
		{
			m_controllers[i]->setJoystickDeadzone(joystickDeadzone);
		}
	}
}
}
