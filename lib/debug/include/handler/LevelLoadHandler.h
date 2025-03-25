#pragma once

#include "DebugEventHandler.h"

namespace diamond_engine
{
	class LevelLoadHandler : public DebugEventHandler
	{
	public:
		~LevelLoadHandler() final override = default;
		std::string doHandleEvent(const std::unique_ptr<DebugEvent>& e) const final override;
	};
}
