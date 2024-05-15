#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BehaviourComponentConfig.h"
#include "RenderComponentConfig.h"

namespace diamond_engine
{
	enum class GameInstanceType : unsigned
	{
		UNKNOWN	= 0,
		SPRITE	= 1
	};

	class GameInstanceConfig
	{
	public:
		GameInstanceType getType() const;
		void setType(GameInstanceType type);

		const std::string& getName() const;
		void setName(const std::string& name);

		const std::vector<std::unique_ptr<RenderComponentConfig>>& getRenderConfigs() const;
		std::vector<std::unique_ptr<RenderComponentConfig>>& getRenderConfigs();
		void setRenderConfigs(std::vector<std::unique_ptr<RenderComponentConfig>> renderConfigs);
		void addRenderConfig(std::unique_ptr<RenderComponentConfig> renderConfig);

		const std::vector<std::unique_ptr<BehaviourComponentConfig>>& getBehaviourConfigs() const;
		std::vector<std::unique_ptr<BehaviourComponentConfig>>& getBehaviourConfigs();
		void setBehaviourConfigs(std::vector<std::unique_ptr<BehaviourComponentConfig>> behaviourConfigs);
		void addBehaviourConfig(std::unique_ptr<BehaviourComponentConfig> behaviourConfig);

	private:
		std::vector<std::unique_ptr<RenderComponentConfig>> m_renderConfigs;
		std::vector<std::unique_ptr<BehaviourComponentConfig>> m_behaviourConfigs;
		std::string m_name{ "Unnamed" };
		GameInstanceType m_type{ GameInstanceType::SPRITE };
	};
}
