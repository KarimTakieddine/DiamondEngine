#pragma once

#include <memory>
#include <sstream>

#include "DebugEvent.h"

namespace diamond_engine
{
	class DebugEventHandler
	{
	public:
		virtual ~DebugEventHandler() = default;
		virtual std::stringstream doHandleEvent(const std::unique_ptr<DebugEvent>& e) const = 0;
	
		std::stringstream handleEvent(std::unique_ptr<DebugEvent> e) const
		{
			if (e->isConsumed())
				return { };

			auto result = doHandleEvent(e);

			e->setConsumed();

			return result;
		}
	};
}
