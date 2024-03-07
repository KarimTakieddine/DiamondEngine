#pragma once

#include <string>

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

	enum class Axis : unsigned
	{
		LEFT	= 0,
		RIGHT	= 1
	};

	using Trigger = Axis;

	class Controller
	{
	public:
		virtual ~Controller() = default;

		virtual bool isConnected() const = 0;
		virtual bool isButtonDown(const std::string& button) const = 0;
		virtual bool isButtonPressed(const std::string& button) const = 0;
		virtual bool isButtonReleased(const std::string& button) const = 0;
		virtual float getAxis(const std::string& axis) const = 0;
		virtual float getTrigger(const std::string& trigger) const = 0;

		virtual void registerButton(const std::string& name, Button button) = 0;
		virtual void registerAxis(const std::string& name, Axis axis) = 0;
		virtual void registerTrigger(const std::string& name, Trigger trigger) = 0;
		virtual void refreshState(unsigned long index) = 0;
	};
}
