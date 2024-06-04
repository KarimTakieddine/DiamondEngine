#pragma once

#include <memory>
#include <future>
#include <unordered_map>
#include <vector>

#include "DebugEvent.h"

namespace diamond_engine
{
	class DebugEventHandler;
	class Debugger
	{
	public:
		~Debugger();

		static std::shared_ptr<Debugger>& getInstance();

		void registerHandler(DebugEvent::Type type, std::unique_ptr<DebugEventHandler> handler);
		void unregisterHandler(DebugEvent::Type type);

		void debugEvent(DebugEvent::Type type, std::unique_ptr<DebugEvent> e);

		void handleAllEvents();

	private:
		Debugger() = default;

		std::unordered_map<DebugEvent::Type, std::unique_ptr<DebugEventHandler>> m_handlers;
		std::vector<std::future<void>> m_outstanding;
	};
}
