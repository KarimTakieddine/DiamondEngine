#pragma once

#include <string>

#include "Size.h"

namespace diamond_engine {
class WindowConfig {
public:
	WindowConfig() = default;

	WindowConfig(const std::string& title, const Size& size);

	void SetTitle(const std::string& title);

	const std::string& GetTitle() const;

	void SetSize(const Size& size);

	const Size& GetSize() const;

private:
	std::string m_title{ "Untitled" };
	Size m_size{ 640, 480 };
};
}
