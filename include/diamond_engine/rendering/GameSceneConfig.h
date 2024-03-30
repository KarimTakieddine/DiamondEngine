#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>

#include "GameInstanceConfig.h"

namespace diamond_engine
{
	class GameSceneConfig
	{
	public:
		void addInstanceConfig(std::unique_ptr<GameInstanceConfig> instanceConfig);
		const std::vector<std::unique_ptr<GameInstanceConfig>>& getInstanceConfigs() const;
		void setInstanceConfigs(std::vector<std::unique_ptr<GameInstanceConfig>> instanceConfigs);

		GLsizei getMaxInstanceCount() const;
		void setMaxInstanceCount(GLsizei maxInstanceCount);

	private:
		std::vector<std::unique_ptr<GameInstanceConfig>> m_instanceConfigs;
		GLsizei m_maxInstanceCount{ 0xFF };
	};
}
