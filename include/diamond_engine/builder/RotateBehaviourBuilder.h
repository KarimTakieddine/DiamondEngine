#pragma once

#include <memory>

#include "Behaviour.h"

namespace diamond_engine {
class BehaviourConfig;

class RotateBehaviourBuilder {
public:
	static std::unique_ptr<Behaviour> Build(const BehaviourConfig* behaviourConfig);
};
}
