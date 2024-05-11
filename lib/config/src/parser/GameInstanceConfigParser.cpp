#include <pugixml.hpp>
#include <string>
#include <unordered_map>

#include "ComponentConfigParser.h"
#include "GameInstanceConfigParser.h"

namespace
{
	using diamond_engine::GameInstanceType;
	const std::unordered_map<std::string, GameInstanceType> kStringToInstanceType = {
		{ "sprite",		GameInstanceType::SPRITE },
		{ "collider",	GameInstanceType::COLLIDER }
	};

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
	std::unique_ptr<GameInstanceConfig> parseGameInstanceConfig(const pugi::xml_node& node, EngineStatus* outStatus /* = nullptr */)
	{
		std::unique_ptr<GameInstanceConfig> result = std::make_unique<GameInstanceConfig>();

		pugi::xml_attribute typeAttribute = node.attribute("type");
		if (typeAttribute)
		{
			const std::string typeString(typeAttribute.as_string());
			auto typeIt = kStringToInstanceType.find(typeString);
			if (typeIt == kStringToInstanceType.cend())
			{
				setErrorStatus(outStatus, "Failed to parse game instance config. Invalid type specified: " + typeString);
				return nullptr;
			}

			result->setType(typeIt->second);
		}

		pugi::xml_node renderComponentsNode = node.child("RenderComponents");
		if (renderComponentsNode)
		{
			for (const auto& renderComponentNode : renderComponentsNode.children())
			{
				result->addRenderConfig(ComponentConfigParser::parseRenderComponent(renderComponentNode, outStatus));

				if (outStatus && !(*outStatus))
				{
					return nullptr;
				}
			}
		}

		pugi::xml_node behaviourComponentsNode = node.child("BehaviourComponents");
		if (behaviourComponentsNode)
		{
			for (const auto& behaviourComponentNode : behaviourComponentsNode.children())
			{
				result->addBehaviourConfig(ComponentConfigParser::parseBehaviourComponent(behaviourComponentNode, outStatus));

				if (outStatus && !(*outStatus))
				{
					return nullptr;
				}
			}
		}

		return result;
	}
}