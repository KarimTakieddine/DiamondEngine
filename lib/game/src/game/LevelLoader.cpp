#include <future>
#include <stdexcept>
#include <vector>

#include "GameSceneConfigParser.h"
#include "LevelLoader.h"

namespace diamond_engine
{
	/* static */ LevelLoader& LevelLoader::getInstance()
	{
		static LevelLoader instance;

		return instance;
	}

	EngineStatus LevelLoader::loadLevels(const std::string& directory)
	{
		std::filesystem::path directoryPath(directory);

		if (!std::filesystem::is_directory(directoryPath))
		{
			return { "Failed to load levels. Path: " + directory + " is not a directory", true };
		}

		using TaskFuture = std::future<LoadTask>;

		std::vector<TaskFuture> taskFutures;
		for (const auto& child : std::filesystem::directory_iterator(directoryPath))
		{
			if (!child.is_regular_file())
			{
				continue;
			}

			TaskFuture taskFut = std::async(std::launch::async, &LevelLoader::runLoadTask, this, new LoadTask{ child.path() });
			taskFutures.push_back(std::move(taskFut));
		}

		std::lock_guard<std::mutex> lock(m_configMutex);
		for (auto& taskFut : taskFutures)
		{
			try
			{
				auto result	= taskFut.get();
				auto& config	= result.config;

				if (!config)
				{
					return { "loadLevels failed - " + result.configPath.string() + " - Error was: " + result.parseStatus.message, true };
				}

				m_configMap.emplace(config->getName(), std::move(config));
			}
			catch (const std::exception& e)
			{
				return { "loadLevels failed: " + std::string(e.what()), true };
			}
		}

		return { };
	}

	const GameSceneConfig* LevelLoader::getLevel(const std::string& name) const
	{
		std::lock_guard<std::mutex> lock(m_configMutex);

		const auto configIt = m_configMap.find(name);

		if (configIt == m_configMap.cend())
		{
			return nullptr;
		}

		return configIt->second.get();
	}

	LevelLoader::LoadTask LevelLoader::runLoadTask(LoadTask* task)
	{
		if (!task)
		{
			throw std::runtime_error("runLoadTask failed. No task object was supplied");
		}

		LoadTask result;

		const std::filesystem::path& configPath = task->configPath;
		
		result.configPath	= configPath;
		result.config		= parseSceneFile(configPath.string(), &(result.parseStatus));

		delete task;

		return result;
	}
}