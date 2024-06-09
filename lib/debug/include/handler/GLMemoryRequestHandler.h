#pragma once

#include "DebugEventHandler.h"

namespace diamond_engine
{
	class GLMemoryRequestHandler : public DebugEventHandler
	{
	public:
		~GLMemoryRequestHandler() final override = default;
		std::string doHandleEvent(const std::unique_ptr<DebugEvent>& e) const final override;
	};
}
