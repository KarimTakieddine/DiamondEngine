#pragma once

#include <memory>
#include <vector>

#include "BehaviourComponentConfig.h"
#include "RenderComponentConfig.h"

namespace diamond_engine
{
	enum class GameInstanceType
	{
		SPRITE = 0
	};

	class GameInstanceConfig
	{
	public:
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
	};
}
