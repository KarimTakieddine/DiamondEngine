#include <chrono>

#include "DebugEventHandler.h"
#include "Debugger.h"
#include "FontEngine.h"

namespace diamond_engine
{
	Debugger::~Debugger()
	{
		handleAllEvents();
	}

	/* static */ std::shared_ptr<Debugger>& Debugger::getInstance()
	{
		static std::shared_ptr<Debugger> debuggerInstance(new Debugger());
		return debuggerInstance;
	}

	void Debugger::registerHandler(DebugEvent::Type type, std::unique_ptr<DebugEventHandler> handler)
	{
		if (m_handlers.find(type) != m_handlers.cend())
			return;

		m_handlers.insert({ type, std::move(handler) });
	}

	void Debugger::unregisterHandler(DebugEvent::Type type)
	{
		if (m_handlers.find(type) == m_handlers.cend())
			return;

		m_handlers.erase(type);
	}

	void Debugger::debugEvent(DebugEvent::Type type, std::unique_ptr<DebugEvent> e)
	{
		auto handlerIt = m_handlers.find(type);

		if (handlerIt == m_handlers.cend())
		{
			e->setConsumed();
			return;
		}

		m_outstanding.push_back(
			std::async(std::launch::async, &DebugEventHandler::handleEvent, handlerIt->second.get(), std::move(e)));
	}

	void Debugger::handleAllEvents()
	{
		for (auto& outstanding : m_outstanding)
		{
			auto currentTime	= std::chrono::steady_clock::now();
			std::future_status waitStatus			= outstanding.wait_until(currentTime + std::chrono::milliseconds(1000));
			
			if (waitStatus == std::future_status::ready)
			{
				try
				{
					FontEngine::getInstance()->printString(outstanding.get(), DEBUG_WINDOW_INDEX);
				}
				catch (const std::exception& e)
				{
					// TODO!
				}
			}
		}

		m_outstanding.clear();
	}

	void Debugger::purgeOutstandingEvents()
	{
		m_outstanding.clear();
	}
}