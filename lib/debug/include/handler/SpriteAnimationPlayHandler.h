#pragma once

#include "DebugEventHandler.h"

namespace diamond_engine
{
	class SpriteAnimationPlayHandler : public DebugEventHandler
	{
	public:
		~SpriteAnimationPlayHandler() final override = default;
		std::string doHandleEvent(const std::unique_ptr<DebugEvent>& e) const final override;
	};
}
