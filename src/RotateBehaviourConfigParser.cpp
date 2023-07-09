#include <stdexcept>

#include <pugixml.hpp>

#include "RotateBehaviourConfig.h"
#include "RotateBehaviourConfigParser.h"

namespace diamond_engine {
	std::unique_ptr<BehaviourConfig> RotateBehaviourConfigParser::Parse(const pugi::xml_node& behaviourNode) {
		std::unique_ptr<RotateBehaviourConfig> result = std::make_unique<RotateBehaviourConfig>();

		pugi::xml_attribute xAnglePerSecAttribute = behaviourNode.attribute("xAnglePerSec");
		if (xAnglePerSecAttribute) {
			result->SetXAnglePerSecond(xAnglePerSecAttribute.as_float());
		}

		pugi::xml_attribute yAnglePerSecAttribute = behaviourNode.attribute("yAnglePerSec");
		if (yAnglePerSecAttribute) {
			result->SetYAnglePerSecond(yAnglePerSecAttribute.as_float());
		}

		pugi::xml_attribute zAnglePerSecAttribute = behaviourNode.attribute("zAnglePerSec");
		if (zAnglePerSecAttribute) {
			result->SetZAnglePerSecond(zAnglePerSecAttribute.as_float());
		}

		return result;
	}
}