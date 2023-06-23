#pragma once

#include "ComponentConfigParser.h"

namespace diamond_engine {
	class MeshRendererConfigParser : public ComponentConfigParser {
	public:
		std::unique_ptr<ComponentConfig> Parse(const pugi::xml_node& componentNode) override;
	};
}
