#pragma once

#include <unordered_map>
#include <vector>

#include "Controller.h"
#include "Key.h"

namespace diamond_engine
{
	class XInputController : public Controller
	{
	public:
		struct JoystickInput
		{
			Joystick joystick;
			glm::vec2 input;
		};

		static const std::unordered_map<Button, unsigned short> kButtonToMaskMap;

		~XInputController() override = default;

		bool isConnected() const override;

		bool isButtonDown(const std::string& button) const override;

		bool isButtonPressed(const std::string& button) const override;

		bool isButtonReleased(const std::string& button) const override;

		glm::vec2 getJoystickInput(const std::string& joystick) const override;

		float getTriggerInput(const std::string& trigger) const override;

		void setJoystickDeadzone(float joystickDeadzone) override;

		void registerButton(const std::string& name, Button button) override;

		void registerJoystick(const std::string& name, Joystick joystick) override;

		void registerTrigger(const std::string& name, Trigger trigger) override;

		void refreshState(unsigned long index) override;

	private:
		std::unordered_map<std::string, input::Key> m_stringToButtonMap;
		std::unordered_map<std::string, JoystickInput> m_stringToJoystickInputMap;
		std::vector<std::string> m_registeredButtons;
		std::vector<std::string> m_registeredJoysticks;
		unsigned long m_packetNumber{ 0 };
		float m_joystickDeadzone{ 0.0f };
		bool m_isConnected{ false };
	};
}
