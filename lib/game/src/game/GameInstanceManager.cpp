#include <algorithm>

#include "GameInstanceConfig.h"
#include "GameInstanceManager.h"

namespace diamond_engine
{
	/* static */ std::shared_ptr<GameInstanceManager>& GameInstanceManager::getInstance()
	{
		static std::shared_ptr<GameInstanceManager> gameManagerInstance( new GameInstanceManager() );

		return gameManagerInstance;
	}

	/* static */ void GameInstanceManager::updateGameInstances(GLfloat deltaTime, const std::vector<std::unique_ptr<GameInstance>>& instances)
	{
		for (const auto& instance : instances)
		{
			if (!instance->isActive())
			{
				continue;
			}

			for (const auto& behaviour : instance->getBehaviourComponents())
			{
				behaviour->update(deltaTime);
			}
		}
	}

	EngineStatus GameInstanceManager::registerInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig)
	{
		if (!instance)
			return { "Failed to register game instance. No allocated instance was provided", true };

		instance->setName(instanceConfig->getName());

		const std::string& instanceName = instance->getName();
		int instanceCount = std::count_if(
			m_registeredInstances.cbegin(),
			m_registeredInstances.cend(),
			[instanceName](const auto& pair) { return pair.second.name == instanceName; });

		instance->setInternalName( instanceCount == 0 ? instanceName : ( instanceName + "_" + std::to_string(instanceCount) ) );

		const std::string& internalName = instance->getInternalName();

		if (m_registeredInstances.find(internalName) != m_registeredInstances.cend())
			return { "Failed to register game instance. Already registered: " + internalName, true };

		m_registeredInstances.insert({ internalName, { instanceName, instance.get() } });

		return { };
	}

	EngineStatus GameInstanceManager::unregisterInstance(const std::string& name)
	{
		auto instanceIt = m_registeredInstances.find(name);

		if (instanceIt == m_registeredInstances.cend())
		{
			return { "Failed to unregister instance. Not found: " + name, true };
		}

		m_registeredInstances.erase(instanceIt);

		return { };
	}

	GameInstance* GameInstanceManager::findGameInstance(const std::string& name)
	{
		auto instanceIt = m_registeredInstances.find(name);

		return instanceIt == m_registeredInstances.cend() ? nullptr : instanceIt->second.instance;
	}
}