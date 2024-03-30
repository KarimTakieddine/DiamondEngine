#pragma once

#include <memory>
#include <string>
#include <vector>

namespace pugi { class xml_document; class xml_node; }

namespace diamond_engine
{
	struct EngineStatus;
	class GameInstanceConfig;
	class GameSceneConfig;
	class GameSceneConfigParser
	{
	public:
		static std::unique_ptr<GameSceneConfig> parseFile(const std::string& file, EngineStatus* outStatus = nullptr);
		static std::unique_ptr<GameSceneConfig> parse(const pugi::xml_document& document, EngineStatus* outStatus = nullptr);
		static std::vector<std::unique_ptr<GameInstanceConfig>> parseInstances(const pugi::xml_node& instancesNode, EngineStatus* outStatus = nullptr);
	};
}
