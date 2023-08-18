#include "KeyboardConfig.h"

namespace diamond_engine {
	const std::vector<KeyConfig>& KeyboardConfig::GetKeyConfigs() const {
		return m_keyConfigs;
	}

	void KeyboardConfig::AddKeyConfig(const KeyConfig& keyConfig) {
		m_keyConfigs.push_back(keyConfig);
	}
}