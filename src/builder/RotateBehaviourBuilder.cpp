#include <stdexcept>

#include "RotateBehaviour.h"
#include "RotateBehaviourBuilder.h"
#include "RotateBehaviourConfig.h"

namespace diamond_engine {
std::unique_ptr<Behaviour> RotateBehaviourBuilder::Build(const BehaviourConfig* behaviourConfig) {
	const RotateBehaviourConfig* rotateBehaviourConfig = dynamic_cast<const RotateBehaviourConfig*>(behaviourConfig);

	if (!rotateBehaviourConfig) {
		throw std::runtime_error("Could not convert BehaviourConfig to target RotateBehaviourConfig");
	}

	std::unique_ptr<RotateBehaviour> result = std::make_unique<RotateBehaviour>();

	result->SetXAnglePerSecond(rotateBehaviourConfig->GetXAnglePerSecond());
	result->SetYAnglePerSecond(rotateBehaviourConfig->GetYAnglePerSecond());
	result->SetZAnglePerSecond(rotateBehaviourConfig->GetZAnglePerSecond());

	return result;
}
}