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

	const std::string& GameSceneConfig::getName() const
	{
		return m_name;
	}

	void GameSceneConfig::setName(const std::string& name)
	{
		m_name = name;
	}

	const glm::vec4& GameSceneConfig::getBackgroundColor() const
	{
		return m_backgroundColor;
	}

	glm::vec4& GameSceneConfig::getBackgroundColor()
	{
		return m_backgroundColor;
	}

	void GameSceneConfig::setBackgroundColor(const glm::vec4& backgroundColor)
	{
		m_backgroundColor = backgroundColor;
	}
}