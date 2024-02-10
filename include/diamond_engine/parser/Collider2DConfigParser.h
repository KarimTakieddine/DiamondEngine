#pragma once

#include <memory>
#include <pugixml.hpp>

#include "ComponentConfig.h"

namespace diamond_engine {
	class Collider2DConfigParser {
	public:
		static std::unique_ptr<ComponentConfig> Parse(const pugi::xml_node& componentNode);
	};
}
