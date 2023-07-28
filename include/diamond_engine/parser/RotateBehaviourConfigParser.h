#pragma once

#include <memory>

#include "config/BehaviourConfig.h"

namespace pugi { class xml_node;  }

namespace diamond_engine {
class RotateBehaviourConfigParser {
public:
	static std::unique_ptr<BehaviourConfig> Parse(const pugi::xml_node& behaviourNode);
};
}
