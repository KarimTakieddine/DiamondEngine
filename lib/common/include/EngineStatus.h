#pragma once

#include <string>

namespace diamond_engine
{
	struct EngineStatus
	{
		std::string message{ "NO_ERROR" };
		bool error{ false };

		operator bool() const
		{
			return !error;
		}
	};
}
