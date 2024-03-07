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
		static const std::unordered_map<Button, unsigned short> kButtonToMaskMap;

		~XInputController() override = default;

		bool isConnected() const override;

		bool isButtonDown(const std::string& button) const override;

		bool isButtonPressed(const std::string& button) const override;

		bool isButtonReleased(const std::string& button) const override;

		float getAxis(const std::string& axis) const override;

		float getTrigger(const std::string& trigger) const override;

		void registerButton(const std::string& name, Button button) override;

		void registerAxis(const std::string& name, Axis axis) override;

		void registerTrigger(const std::string& name, Trigger trigger) override;

		void refreshState(unsigned long index) override;

	private:
		std::unordered_map<std::string, input::Key> m_stringToButtonMap;
		std::vector<std::string> m_registeredButtons;
		unsigned long m_packetNumber{ 0 };
		bool m_isConnected{ false };
	};
}
