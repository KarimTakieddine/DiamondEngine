#pragma once

#include "DebugEventHandler.h"

namespace diamond_engine
{
	class GLAllocationEventHandler : public DebugEventHandler
	{
	public:
		~GLAllocationEventHandler() final override = default;
		std::string doHandleEvent(const std::unique_ptr<DebugEvent>& e) const final override;
	};
}
