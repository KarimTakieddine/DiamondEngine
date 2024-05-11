#include "WindowConfig.h"

namespace diamond_engine {
	WindowConfig::WindowConfig(const std::string& title, const Size& size) : m_title(title), m_size(size) { }

	void WindowConfig::SetTitle(const std::string& title) {
		m_title = title;
	}

	const std::string& WindowConfig::GetTitle() const {
		return m_title;
	}

	void WindowConfig::SetSize(const Size& size) {
		m_size = size;
	}

	const Size& WindowConfig::GetSize() const {
		return m_size;
	}
}