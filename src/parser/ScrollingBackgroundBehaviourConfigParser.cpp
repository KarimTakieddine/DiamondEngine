#include <stdexcept>

#include <pugixml.hpp>

#include "ScrollingBackgroundBehaviourConfig.h"
#include "ScrollingBackgroundBehaviourConfigParser.h"
#include "Vector2Parser.h"

namespace diamond_engine {
	std::unique_ptr<BehaviourConfig> ScrollingBackgroundBehaviourConfigParser::Parse(const pugi::xml_node& behaviourNode) {
		std::unique_ptr<ScrollingBackgroundBehaviourConfig> result = std::make_unique<ScrollingBackgroundBehaviourConfig>();

		result->setScrollingSpeed(Vector2Parser::Parse(behaviourNode));

		return result;
	}
}