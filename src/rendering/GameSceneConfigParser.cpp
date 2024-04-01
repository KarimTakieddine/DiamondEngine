#include <pugixml.hpp>

#include "EngineStatus.h"
#include "GameInstanceConfigParser.h"
#include "GameSceneConfig.h"
#include "GameSceneConfigParser.h"
#include "Vector3Parser.h"

namespace diamond_engine
{
	/* static */ std::unique_ptr<GameSceneConfig> GameSceneConfigParser::parseFile(const std::string& file, EngineStatus* outStatus)
	{
		pugi::xml_document document;
		pugi::xml_parse_result parseResult = document.load_file(file.c_str(), pugi::parse_default, pugi::encoding_utf8);
		if (!parseResult)
		{
			if (outStatus)
				*outStatus = { parseResult.description(), true };

			return nullptr;
		}

		return parse(document, outStatus);
	}

	/* static */ std::unique_ptr<GameSceneConfig> GameSceneConfigParser::parse(const pugi::xml_document& document, EngineStatus* outStatus)
	{
		pugi::xml_node rootNode = document.root().first_child();

		if (!rootNode)
		{
			if (outStatus)
				*outStatus = { "Failed to parse GameSceneConfig. No root node was provided", true };

			return nullptr;
		}

		std::unique_ptr<GameSceneConfig> result = std::make_unique<GameSceneConfig>();

		pugi::xml_attribute maxInstanceCountAttribute = rootNode.attribute("maxInstanceCount");
		if (maxInstanceCountAttribute)
		{
			result->setMaxInstanceCount(maxInstanceCountAttribute.as_int(result->getMaxInstanceCount()));
		}

		pugi::xml_node backgroundColorNode = rootNode.child("BackgroundColor");
		if (backgroundColorNode)
		{
			result->setBackgroundColor(Vector3Parser::Parse(backgroundColorNode));
		}

		pugi::xml_node instancesNode = rootNode.child("Instances");
		if (instancesNode)
		{
			result->setInstanceConfigs(parseInstances(instancesNode, outStatus));
		}

		if (outStatus)
			outStatus = { };

		return result;
	}

	/* static */ std::vector<std::unique_ptr<GameInstanceConfig>> GameSceneConfigParser::parseInstances(const pugi::xml_node& instancesNode, EngineStatus* outStatus)
	{
		std::vector<std::unique_ptr<GameInstanceConfig>> result;
		for (const auto& instanceNode : instancesNode.children("Instance"))
		{
			result.push_back(GameInstanceConfigParser::parse(instanceNode, outStatus));

			if (outStatus && !(*outStatus))
			{
				return result;
			}
		}

		if (outStatus)
			*outStatus = { };

		return result;
	}
}