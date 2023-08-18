#pragma once

#include <algorithm>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "Keyboard.h"

namespace diamond_engine {
namespace input {
	/* static */ const std::unordered_map<GLFWKeyCode, int> Keyboard::kGLFWKeyCodeMap = {
		{ GLFWKeyCode::KEYCODE_ESCAPE,	GLFW_KEY_ESCAPE },
		{ GLFWKeyCode::KEYCODE_SPACE,	GLFW_KEY_SPACE },
		{ GLFWKeyCode::KEYCODE_UP,		GLFW_KEY_UP },
		{ GLFWKeyCode::KEYCODE_DOWN,	GLFW_KEY_DOWN },
		{ GLFWKeyCode::KEYCODE_LEFT,	GLFW_KEY_LEFT },
		{ GLFWKeyCode::KEYCODE_RIGHT,	GLFW_KEY_RIGHT }
	};

	Keyboard::Keyboard() : m_keyMap() {
		m_keyMap.reserve(kMaxKeyCount);
		m_keyNames.reserve(kMaxKeyCount);
	}

	void Keyboard::RegisterKey(const std::string& name, GLFWKeyCode code) {
		auto glfwKeyCodeIt = kGLFWKeyCodeMap.find(code);

		if (glfwKeyCodeIt == kGLFWKeyCodeMap.cend()) {
			throw std::runtime_error("Invalid GLFWKeyCode specified: " + std::to_string(static_cast<std::underlying_type_t<GLFWKeyCode>>(code)));
		}

		auto keyIt = m_keyMap.find(name);
		const bool keyExists = (keyIt != m_keyMap.cend());

		m_keyMap.emplace(name, Key{ glfwKeyCodeIt->second, KeyState::NONE });
		
		if (!keyExists) {
			m_keyNames.push_back(name);
		}
	}

	void Keyboard::UnregisterKey(const std::string& name) {
		auto keyIt = m_keyMap.find(name);

		if (keyIt == m_keyMap.cend()) {
			return;
		}

		m_keyMap.erase(keyIt);
		m_keyNames.erase(std::remove_if(m_keyNames.begin(), m_keyNames.end(), [&](const std::string& keyName) { return keyName == name; }), m_keyNames.end());
	}

	void Keyboard::UpdateKeyStates(GLFWwindow* window) {
		for (const auto& keyName : m_keyNames) {
			Key& key = m_keyMap.at(keyName);

			switch (glfwGetKey(window, key.GetCode())) {
			case GLFW_PRESS: {
				key.UnsetState(KeyState::RELEASED | KeyState::WAS_RELEASED_LAST_FRAME);

				if (key.WasPressedLastFrame()) {
					key.UnsetState(KeyState::PRESSED_DOWN);
				}
				else {
					key.SetState(KeyState::PRESSED_DOWN);
					key.SetState(KeyState::WAS_PRESSED_LAST_FRAME);
				}

				key.SetState(KeyState::PRESSED);

				break;
			}
			case GLFW_RELEASE: {
				key.UnsetState(KeyState::PRESSED | KeyState::PRESSED_DOWN | KeyState::WAS_PRESSED_LAST_FRAME);

				if (key.WasReleasedLastFrame()) {
					key.UnsetState(KeyState::RELEASED);
				}
				else {
					key.SetState(KeyState::RELEASED);
					key.SetState(KeyState::WAS_RELEASED_LAST_FRAME);
				}
				
				break;
			}
			default: {
				key.ClearState();
				break;
			}
			}
		}
	}

	bool Keyboard::IsKeyDown(const std::string& name) const {
		auto keyIt = m_keyMap.find(name);

		if (keyIt == m_keyMap.cend()) {
			return false;
		}

		return keyIt->second.IsPressedDown();
	}

	bool Keyboard::IsKeyPressed(const std::string& name) const {
		auto keyIt = m_keyMap.find(name);

		if (keyIt == m_keyMap.cend()) {
			return false;
		}

		return keyIt->second.IsPressed();
	}

	bool Keyboard::IsKeyReleased(const std::string& name) const {
		auto keyIt = m_keyMap.find(name);

		if (keyIt == m_keyMap.cend()) {
			return false;
		}

		return keyIt->second.IsReleased();
	}
}
}