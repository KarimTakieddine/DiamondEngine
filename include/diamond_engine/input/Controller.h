#pragma once

#include <string>

#include <glm/vec2.hpp>

namespace diamond_engine
{
	enum class Button : int
	{
		BUTTON_0 = 0, // A/X button
		BUTTON_1 = 1, // X/[] button
		BUTTON_2 = 2, // Y/^ button
		BUTTON_3 = 3, // B/O button
		BUTTON_4 = 4, // LB/L1 button
		BUTTON_5 = 5, // RB/R1 button
		BUTTON_6 = 6, // START/OPTIONS button
		BUTTON_7 = 7  // SELECT/SHARE button
	};

	enum class Joystick : short
	{
		LEFT	= 0,
		RIGHT	= 1
	};

	using Trigger = Joystick;

	class Controller
	{
	public:
		virtual ~Controller() = default;

		virtual bool isConnected() const = 0;
		virtual bool isButtonDown(const std::string& button) const = 0;
		virtual bool isButtonPressed(const std::string& button) const = 0;
		virtual bool isButtonReleased(const std::string& button) const = 0;
		virtual glm::vec2 getJoystickInput(const std::string& joystick) const = 0;
		virtual float getTriggerInput(const std::string& trigger) const = 0;
		
		virtual void setJoystickDeadzone(float joystickDeadzone) = 0;
		virtual void registerButton(const std::string& name, Button button) = 0;
		virtual void registerJoystick(const std::string& name, Joystick joystick) = 0;
		virtual void registerTrigger(const std::string& name, Trigger trigger) = 0;
		virtual void refreshState(unsigned long index) = 0;
	};
}
