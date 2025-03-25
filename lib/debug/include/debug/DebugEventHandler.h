#pragma once

#include <memory>
#include <string>

#include "DebugEvent.h"

namespace diamond_engine
{
	class DebugEventHandler
	{
	public:
		virtual ~DebugEventHandler() = default;
		virtual std::string doHandleEvent(const std::unique_ptr<DebugEvent>& e) const = 0;
	
		std::string handleEvent(std::unique_ptr<DebugEvent> e) const
		{
			if (e->isConsumed())
				return { };

			auto result = doHandleEvent(e);

			e->setConsumed();

			return result;
		}
	};
}
