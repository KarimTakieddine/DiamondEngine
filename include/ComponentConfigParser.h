#pragma once

#include <memory>

namespace pugi { class xml_node; }

namespace diamond_engine {
	class ComponentConfig;

	class ComponentConfigParser {
	public:
		virtual std::unique_ptr<ComponentConfig> Parse(const pugi::xml_node& componentNode) = 0;
	};
}
