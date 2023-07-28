#pragma once

#include <memory>
#include <pugixml.hpp>

#include "config/ComponentConfig.h"

namespace diamond_engine {
class MeshRendererConfigParser {
public:
	static std::unique_ptr<ComponentConfig> Parse(const pugi::xml_node& componentNode);
};
}
