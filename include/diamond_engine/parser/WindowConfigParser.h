#pragma once

#include "WindowConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class WindowConfigParser {
public:
	static WindowConfig Parse(const pugi::xml_node& windowConfigNode);
};
}
