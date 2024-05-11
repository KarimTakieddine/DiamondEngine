#pragma once

#include <string>
#include <vector>

#include "KeyCode.h"

namespace diamond_engine
{
	struct KeyConfig {
		std::string name;
		KeyCode code;
	};

	class KeyboardConfig {
	public:
		const std::vector<KeyConfig>& GetKeyConfigs() const;

		void AddKeyConfig(const KeyConfig& keyConfig);

	private:
		std::vector<KeyConfig> m_keyConfigs;
	};
}
