#pragma once

#include <memory>

#include "BehaviourComponentConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine {
	class SpriteAnimationPlayerConfigParser {
	public:
		static std::unique_ptr<BehaviourComponentConfig> Parse(const pugi::xml_node& componentNode);
	};
}
