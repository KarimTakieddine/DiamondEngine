#include "GameSceneConfig.h"

namespace diamond_engine
{
	void GameSceneConfig::addInstanceConfig(std::unique_ptr<GameInstanceConfig> instanceConfig)
	{
		m_instanceConfigs.push_back(std::move(instanceConfig));
	}

	const std::vector<std::unique_ptr<GameInstanceConfig>>& GameSceneConfig::getInstanceConfigs() const
	{
		return m_instanceConfigs;
	}

	void GameSceneConfig::setInstanceConfigs(std::vector<std::unique_ptr<GameInstanceConfig>> instanceConfigs)
	{
		m_instanceConfigs = std::move(instanceConfigs);
	}

	GLsizei GameSceneConfig::getMaxInstanceCount() const
	{
		return m_maxInstanceCount;
	}

	void GameSceneConfig::setMaxInstanceCount(GLsizei maxInstanceCount)
	{
		m_maxInstanceCount = maxInstanceCount;
	}
}