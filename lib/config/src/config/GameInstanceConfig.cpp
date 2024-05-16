#include "GameInstanceConfig.h"

namespace diamond_engine
{
	GameInstanceType GameInstanceConfig::getType() const
	{
		return m_type;
	}

	void GameInstanceConfig::setType(GameInstanceType type)
	{
		m_type = type;
	}

	const std::string& GameInstanceConfig::getName() const
	{
		return m_name;
	}

	void GameInstanceConfig::setName(const std::string& name)
	{
		m_name = name;
	}

	const std::vector<std::unique_ptr<RenderComponentConfig>>& GameInstanceConfig::getRenderConfigs() const
	{
		return m_renderConfigs;
	}

	std::vector<std::unique_ptr<RenderComponentConfig>>& GameInstanceConfig::getRenderConfigs()
	{
		return m_renderConfigs;
	}

	void GameInstanceConfig::setRenderConfigs(std::vector<std::unique_ptr<RenderComponentConfig>> renderConfigs)
	{
		m_renderConfigs = std::move(renderConfigs);
	}

	void GameInstanceConfig::addRenderConfig(std::unique_ptr<RenderComponentConfig> renderConfig)
	{
		m_renderConfigs.push_back(std::move(renderConfig));
	}

	const std::vector<std::unique_ptr<BehaviourComponentConfig>>& GameInstanceConfig::getBehaviourConfigs() const
	{
		return m_behaviourConfigs;
	}

	std::vector<std::unique_ptr<BehaviourComponentConfig>>& GameInstanceConfig::getBehaviourConfigs()
	{
		return m_behaviourConfigs;
	}

	void GameInstanceConfig::setBehaviourConfigs(std::vector<std::unique_ptr<BehaviourComponentConfig>> behaviourConfigs)
	{
		m_behaviourConfigs = std::move(behaviourConfigs);
	}

	void GameInstanceConfig::addBehaviourConfig(std::unique_ptr<BehaviourComponentConfig> behaviourConfig)
	{
		m_behaviourConfigs.push_back(std::move(behaviourConfig));
	}
}