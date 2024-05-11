#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "Key.h"
#include "KeyboardConfig.h"

struct GLFWwindow;

namespace diamond_engine {
namespace input {
class Keyboard {
public:
	static constexpr size_t kMaxKeyCount = 0x68;

	static const std::unordered_map<GLFWKeyCode, int> kGLFWKeyCodeMap;

	using KeyMap = std::unordered_map<std::string, Key>;

	Keyboard();

	void RegisterKey(const std::string& name, GLFWKeyCode code);

	void UnregisterKey(const std::string& name);

	void UpdateKeyStates(GLFWwindow* window);

	bool IsKeyDown(const std::string& name) const;

	bool IsKeyPressed(const std::string& name) const;

	bool IsKeyReleased(const std::string& name) const;

private:
	KeyMap m_keyMap;
	std::vector<std::string> m_keyNames;
};
}
}
