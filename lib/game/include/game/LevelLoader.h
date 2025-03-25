#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "EngineStatus.h"
#include "GameSceneConfig.h"

namespace diamond_engine
{
	class LevelLoader
	{
	public:
		struct LoadTask
		{
			std::filesystem::path configPath;
			std::unique_ptr<GameSceneConfig> config;
			EngineStatus parseStatus;
		};

		static LevelLoader& getInstance();

		EngineStatus loadLevels(const std::string& directory);

		const GameSceneConfig* getLevel(const std::string& name) const;

	private:
		LevelLoader() = default;

		LoadTask runLoadTask(LoadTask* task);

		mutable std::mutex m_configMutex;
		std::unordered_map<std::string, std::unique_ptr<GameSceneConfig>> m_configMap;
	};
}
