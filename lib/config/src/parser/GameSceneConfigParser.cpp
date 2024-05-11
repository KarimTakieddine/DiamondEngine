#include <pugixml.hpp>

#include "GameInstanceConfigParser.h"
#include "GameSceneConfigParser.h"
#include "Vector3Parser.h"

namespace
{
	using diamond_engine::EngineStatus;
	static void setErrorStatus(EngineStatus* status, const std::string& message, bool error = true)
	{
		if (!status)
		{
			return;
		}

		*status = { message, error };
	}
}

namespace diamond_engine
{
	std::unique_ptr<GameSceneConfig> parseSceneFile(const std::string& file, EngineStatus* outStatus /* = nullptr */)
	{
		pugi::xml_document document;
		pugi::xml_parse_result parseResult = document.load_file(file.c_str(), pugi::parse_default, pugi::encoding_utf8);
		if (!parseResult)
		{
			setErrorStatus(outStatus, parseResult.description());
			return nullptr;
		}

		return parseSceneDocument(document, outStatus);
	}

	std::unique_ptr<GameSceneConfig> parseSceneDocument(const pugi::xml_document& document, EngineStatus* outStatus /* = nullptr */)
	{
		pugi::xml_node rootNode = document.root().first_child();

		if (!rootNode)
		{
			setErrorStatus(outStatus, "Failed to parse game scene config. No root node was provided");
			return nullptr;
		}

		return parseScene(rootNode);
	}

	std::unique_ptr<GameSceneConfig> parseScene(const pugi::xml_node& node, EngineStatus* outStatus /* = nullptr*/)
	{
		std::unique_ptr<GameSceneConfig> result = std::make_unique<GameSceneConfig>();

		pugi::xml_attribute maxInstanceCountAttribute = node.attribute("maxInstanceCount");
		if (maxInstanceCountAttribute)
		{
			result->setMaxInstanceCount(maxInstanceCountAttribute.as_int(result->getMaxInstanceCount()));
		}

		pugi::xml_node backgroundColorNode = node.child("BackgroundColor");
		if (backgroundColorNode)
		{
			result->setBackgroundColor(Vector3Parser::Parse(backgroundColorNode));
		}

		pugi::xml_node instancesNode = node.child("Instances");
		if (instancesNode)
		{
			for (const auto& instanceNode : instancesNode.children("Instance"))
			{
				result->addInstanceConfig(parseGameInstanceConfig(instanceNode, outStatus));
			}
		}

		return result;
	}
}