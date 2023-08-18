#pragma once

#include "Size.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class SizeParser {
public:
	static Size Parse(const pugi::xml_node& sizeNode);
};
}
