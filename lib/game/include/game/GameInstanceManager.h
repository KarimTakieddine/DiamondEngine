#pragma once

#include <map>
#include <string>
#include <memory>

#include "GameInstance.h"

namespace diamond_engine
{
	struct ManagedGameInstance
	{
		std::string name{ "Unnamed" };
		GameInstance* instance{ nullptr };
	};

	class GameInstanceConfig;
	class GameInstanceManager
	{
	public:
		static std::shared_ptr<GameInstanceManager>& getInstance();

		static void updateGameInstances(GLfloat deltaTime, const std::vector<std::unique_ptr<GameInstance>>& instances);
		EngineStatus registerInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig);
		EngineStatus unregisterInstance(const std::string& name);
		GameInstance* findGameInstance(const std::string& name);

	private:
		GameInstanceManager() = default;

		std::map<std::string, ManagedGameInstance> m_registeredInstances;
	};
}
