#pragma once

#include <map>
#include <string>
#include <memory>

#include "GameInstance.h"

namespace diamond_engine
{
	class GameInstanceConfig;
	class GameInstanceManager
	{
	public:
		EngineStatus registerInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig);
		EngineStatus unregisterInstance(const std::string& name);

	private:
		std::map<std::string, std::string> m_registeredInstances;
	};
}
