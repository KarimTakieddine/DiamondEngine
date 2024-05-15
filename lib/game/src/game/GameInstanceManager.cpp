#include <algorithm>

#include "GameInstanceConfig.h"
#include "GameInstanceManager.h"

namespace diamond_engine
{
	EngineStatus GameInstanceManager::registerInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig)
	{
		if (!instance)
			return { "Failed to register game instance. No allocated instance was provided", true };

		instance->setName(instanceConfig->getName());

		const std::string& instanceName = instance->getName();
		int instanceCount = std::count_if(
			m_registeredInstances.cbegin(),
			m_registeredInstances.cend(),
			[instanceName](const auto& pair) { return pair.second == instanceName; });

		instance->setInternalName(instanceName + "_" + std::to_string(instanceCount));

		const std::string& internalName = instance->getInternalName();

		if (m_registeredInstances.find(internalName) != m_registeredInstances.cend())
			return { "Failed to register game instance. Already registered: " + internalName, true };

		m_registeredInstances.insert({ instance->getInternalName(), instance->getName() });

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
}