#include <algorithm>

#include <Windows.h>
#include <xinput.h>

#include "XInputController.h"

namespace diamond_engine
{
	/* static */ const std::unordered_map<Button, unsigned short> XInputController::kButtonToMaskMap =
	{
		{ Button::BUTTON_0, XINPUT_GAMEPAD_A },
		{ Button::BUTTON_1, XINPUT_GAMEPAD_X },
		{ Button::BUTTON_2, XINPUT_GAMEPAD_Y },
		{ Button::BUTTON_3, XINPUT_GAMEPAD_B },
		{ Button::BUTTON_4, XINPUT_GAMEPAD_LEFT_SHOULDER },
		{ Button::BUTTON_5, XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ Button::BUTTON_6, XINPUT_GAMEPAD_START },
		{ Button::BUTTON_7, XINPUT_GAMEPAD_BACK }
	};

	bool XInputController::isConnected() const
	{
		return m_isConnected;
	}

	bool XInputController::isButtonDown(const std::string& button) const
	{
		auto buttonIt = m_stringToButtonMap.find(button);

		if (buttonIt == m_stringToButtonMap.cend())
		{
			return false;
		}

		return buttonIt->second.IsPressedDown();
	}

	bool XInputController::isButtonPressed(const std::string& button) const
	{
		auto buttonIt = m_stringToButtonMap.find(button);

		if (buttonIt == m_stringToButtonMap.cend())
		{
			return false;
		}

		return buttonIt->second.IsPressed();
	}

	bool XInputController::isButtonReleased(const std::string& button) const
	{
		auto buttonIt = m_stringToButtonMap.find(button);

		if (buttonIt == m_stringToButtonMap.cend())
		{
			return false;
		}

		return buttonIt->second.IsReleased();
	}

	float XInputController::getAxis(const std::string& axis) const
	{
		return 0.0f;
	}

	float XInputController::getTrigger(const std::string& trigger) const
	{
		return 0.0f;
	}

	void XInputController::registerButton(const std::string& name, Button button)
	{
		auto buttonIt = std::find_if(m_registeredButtons.cbegin(), m_registeredButtons.cend(), [name](const std::string& buttonName) { return name == buttonName; });

		if (buttonIt != m_registeredButtons.cend())
		{
			return;
		}

		m_stringToButtonMap.insert({ name, input::Key { static_cast<std::underlying_type_t<Button>>(button), input::KeyState::NONE } });

		m_registeredButtons.push_back(name);
	}

	void XInputController::registerAxis(const std::string& name, Axis axis)
	{

	}

	void XInputController::registerTrigger(const std::string& name, Trigger trigger)
	{

	}

	void XInputController::refreshState(unsigned long index)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		DWORD status = XInputGetState(index, &state);

		if (status != ERROR_SUCCESS)
		{
			// Includes ERROR_DEVICE_NOT_CONNECTED

			for (const auto& registeredButtonName : m_registeredButtons)
			{
				auto buttonIt = m_stringToButtonMap.find(registeredButtonName);

				if (buttonIt == m_stringToButtonMap.cend())
				{
					continue;
				}

				input::Key& buttonKey = buttonIt->second;
				buttonKey.ClearState();
			}

			m_isConnected = false;
			return;
		}

		m_isConnected = true;

		const DWORD packetNumber = state.dwPacketNumber;

		if (packetNumber == m_packetNumber)
		{
			for (const auto& registeredButtonName : m_registeredButtons)
			{
				auto buttonIt = m_stringToButtonMap.find(registeredButtonName);

				if (buttonIt == m_stringToButtonMap.cend())
				{
					continue;
				}

				input::Key& buttonKey = buttonIt->second;

				if (buttonKey.WasPressedLastFrame())
				{
					buttonKey.UnsetState(input::KeyState::PRESSED_DOWN);
				}

				if (buttonKey.WasReleasedLastFrame())
				{
					buttonKey.UnsetState(input::KeyState::RELEASED);
				}
			}

			return;
		}

		const XINPUT_GAMEPAD& gamepad = state.Gamepad;
		
		const WORD buttons = gamepad.wButtons;
		for (const auto& registeredButtonName : m_registeredButtons)
		{
			auto buttonIt = m_stringToButtonMap.find(registeredButtonName);

			if (buttonIt == m_stringToButtonMap.cend())
			{
				continue;
			}

			const Button button = static_cast<Button>(buttonIt->second.GetCode());

			auto maskIt = kButtonToMaskMap.find(button);

			if (maskIt == kButtonToMaskMap.cend())
			{
				continue;
			}

			input::Key& buttonKey = buttonIt->second;

			if (buttons & maskIt->second)
			{
				buttonKey.UnsetState(input::KeyState::RELEASED | input::KeyState::WAS_RELEASED_LAST_FRAME);

				buttonKey.SetState(input::KeyState::PRESSED_DOWN);
				buttonKey.SetState(input::KeyState::WAS_PRESSED_LAST_FRAME);
				buttonKey.SetState(input::KeyState::PRESSED);
			}
			else
			{
				buttonKey.UnsetState(input::KeyState::PRESSED | input::KeyState::PRESSED_DOWN | input::KeyState::WAS_PRESSED_LAST_FRAME);

				buttonKey.SetState(input::KeyState::RELEASED);
				buttonKey.SetState(input::KeyState::WAS_RELEASED_LAST_FRAME);
			}
		}

		m_packetNumber = packetNumber;
	}
}