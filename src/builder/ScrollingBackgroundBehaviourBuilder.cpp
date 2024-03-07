#include <stdexcept>

#include "ScrollingBackgroundBehaviour.h"
#include "ScrollingBackgroundBehaviourBuilder.h"
#include "ScrollingBackgroundBehaviourConfig.h"


namespace diamond_engine
{
	/* static */ std::unique_ptr<Behaviour> ScrollingBackgroundBehaviourBuilder::Build(const BehaviourConfig* behaviourConfig)
	{
		const ScrollingBackgroundBehaviourConfig* scrollingBackgroundBehaviourConfig = dynamic_cast<const ScrollingBackgroundBehaviourConfig*>(behaviourConfig);

		if (!scrollingBackgroundBehaviourConfig) {
			throw std::runtime_error("Could not convert BehaviourConfig to target ScrollingBackgroundBehaviourConfig");
		}

		std::unique_ptr<ScrollingBackgroundBehaviour> result = std::make_unique<ScrollingBackgroundBehaviour>();

		result->setScrollingSpeed(scrollingBackgroundBehaviourConfig->getScrollingSpeed());

		return result;
	}
}