#pragma once

#include <type_traits>

namespace diamond_engine {
namespace input {
enum class KeyState : unsigned char {
	NONE					= 0,
	PRESSED_DOWN			= 1,
	PRESSED					= 1 << 1,
	WAS_PRESSED_LAST_FRAME	= 1 << 2,
	RELEASED				= 1 << 3,
	WAS_RELEASED_LAST_FRAME	= 1 << 4
};

inline KeyState operator|(KeyState lhs, KeyState rhs) {
	return static_cast<KeyState>(
		static_cast<std::underlying_type_t<KeyState>>(lhs) |
		static_cast<std::underlying_type_t<KeyState>>(rhs));
}

inline KeyState& operator|=(KeyState& l_value, KeyState value) {
	l_value = l_value | value;
	return l_value;
}

inline KeyState operator~(KeyState state) {
	return static_cast<KeyState>(~(static_cast<std::underlying_type_t<KeyState>>(state)));
}

inline KeyState operator&(KeyState lhs, KeyState rhs) {
	return static_cast<KeyState>(
		static_cast<std::underlying_type_t<KeyState>>(lhs) &
		static_cast<std::underlying_type_t<KeyState>>(rhs));
}

inline KeyState& operator&=(KeyState& l_value, KeyState value) {
	l_value = l_value & value;
	return l_value;
}

class Key {
public:
	Key(int code, KeyState state) : m_code(code), m_state(state) { }

	int GetCode() const {
		return m_code;
	}

	void ClearState() {
		m_state = KeyState::NONE;
	}
	
	void SetState(KeyState state) {
		m_state |= state;
	}

	void UnsetState(KeyState state) {
		m_state &= ~state;
	}

	KeyState GetState() const {
		return m_state;
	}

	bool IsPressed() const {
		return (m_state & KeyState::PRESSED) == KeyState::PRESSED;
	}

	bool IsPressedDown() const {
		return (m_state & KeyState::PRESSED_DOWN) == KeyState::PRESSED_DOWN;
	}

	bool WasPressedLastFrame() const {
		return (m_state & KeyState::WAS_PRESSED_LAST_FRAME) == KeyState::WAS_PRESSED_LAST_FRAME;
	}

	bool IsReleased() const {
		return (m_state & KeyState::RELEASED) == KeyState::RELEASED;
	}

	bool WasReleasedLastFrame() const {
		return (m_state & KeyState::WAS_RELEASED_LAST_FRAME) == KeyState::WAS_RELEASED_LAST_FRAME;
	}


private:
	int m_code{ 0 };
	KeyState m_state{ KeyState::NONE };
};
}
}
