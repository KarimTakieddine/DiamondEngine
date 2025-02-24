#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/vec4.hpp>

#include "GameInstanceConfig.h"

namespace diamond_engine
{
	class GameSceneConfig
	{
	public:
		void addInstanceConfig(std::unique_ptr<GameInstanceConfig> instanceConfig);
		const std::vector<std::unique_ptr<GameInstanceConfig>>& getInstanceConfigs() const;
		void setInstanceConfigs(std::vector<std::unique_ptr<GameInstanceConfig>> instanceConfigs);

		const std::string& getName() const;
		void setName(const std::string& name);

		const glm::vec4& getBackgroundColor() const;
		glm::vec4& getBackgroundColor();
		void setBackgroundColor(const glm::vec4& backgroundColor);

	private:
		std::string m_name{ "UNNAMED" };
		std::vector<std::unique_ptr<GameInstanceConfig>> m_instanceConfigs;
		glm::vec4 m_backgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		GLsizei m_maxInstanceCount{ 0xFF };
	};
}
