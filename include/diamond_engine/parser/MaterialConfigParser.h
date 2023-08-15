#pragma once

#include "MaterialConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
class MaterialConfigParser {
public:
	static MaterialConfig Parse(const pugi::xml_node& node);
};
}
