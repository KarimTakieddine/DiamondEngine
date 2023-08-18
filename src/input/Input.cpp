#include "Input.h"

namespace diamond_engine {
namespace input {
	/* static */ StateMonitor& StateMonitor::GetInstance() {
		static StateMonitor stateMonitorInstance;
		
		return stateMonitorInstance;
	}

	void StateMonitor::MonitorStates(GLFWwindow* window) {
		m_keyboard.UpdateKeyStates(window);
	}

	void StateMonitor::RegisterKeyboardKey(const std::string& name, GLFWKeyCode code) {
		m_keyboard.RegisterKey(name, code);
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

	bool IsKeyPressed(const std::string& keyName) {
		return StateMonitor::GetInstance().IsKeyPressed(keyName);
	}

	bool IsKeyDown(const std::string& keyName) {
		return StateMonitor::GetInstance().IsKeyDown(keyName);
	}

	bool IsKeyReleased(const std::string& keyName) {
		return StateMonitor::GetInstance().IsKeyReleased(keyName);
	}
}
}
