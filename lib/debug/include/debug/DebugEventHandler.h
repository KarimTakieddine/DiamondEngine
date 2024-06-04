#pragma once

#include <memory>

namespace diamond_engine
{
	class DebugEvent;
	class DebugEventHandler
	{
	public:
		virtual ~DebugEventHandler() = default;
		virtual void handleEvent(std::unique_ptr<DebugEvent> e) = 0;
	};
}
