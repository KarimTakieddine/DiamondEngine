#pragma once

#include <memory>

#include "Behaviour.h"

namespace diamond_engine {
	class BehaviourConfig;

	class ScrollingBackgroundBehaviourBuilder {
	public:
		static std::unique_ptr<Behaviour> Build(const BehaviourConfig* behaviourConfig);
	};
}
