#include <stdexcept>

#include <pugixml.hpp>

#include "GameObjectConfigParser.h"
#include "MeshRendererConfigParser.h"
#include "Vector3Parser.h"

namespace diamond_engine {
	static const std::unordered_map<std::string, GameObjectType> kStringToGameObjectType = {
		{ "sprite", GameObjectType::SPRITE }
	};

	/* static */ std::unordered_map<std::string, GameObjectConfigParser::ComponentParseFunc> GameObjectConfigParser::StringToParseMap = {
		{ "MeshRenderer", &MeshRendererConfigParser::Parse }
	};

	/* static */ std::unique_ptr<GameObjectConfig> GameObjectConfigParser::Parse(const pugi::xml_node& gameObjectNode) {
		if (!gameObjectNode) {
			throw std::runtime_error("No <GameObject/> node supplied to GameObjectConfigParser::Parse()");
		}

		pugi::xml_attribute typeAttribute = gameObjectNode.attribute("type");
		if (!typeAttribute) {
			throw std::runtime_error("No \"type\" attribute specified for parsed <GameObject/> node");
		}
		const std::string typeString(typeAttribute.as_string());

		auto typeIt = kStringToGameObjectType.find(typeString);
		if (typeIt == kStringToGameObjectType.cend()) {
			throw std::runtime_error("Invalid GameObject type specified on parsed <GameObject/> node: " + typeString);
		}
		
		std::unique_ptr<GameObjectConfig> gameObjectConfig = std::make_unique<GameObjectConfig>();
		gameObjectConfig->SetType(typeIt->second);

		pugi::xml_node componentsNode = gameObjectNode.child("Components");

		if (componentsNode) {
			for (pugi::xml_node componentNode : componentsNode.children()) {
				const std::string componentNameString(componentNode.name());

				auto componentParseFuncIt = StringToParseMap.find(componentNameString);
				if (componentParseFuncIt == StringToParseMap.end()) {
					throw std::runtime_error("Unknown component name specified: \"" + componentNameString + "\". Did you forget to register the component type?");
				}

				gameObjectConfig->AddComponentConfig(componentParseFuncIt->second(componentNode));
			}
		}

		pugi::xml_node positionNode = gameObjectNode.child("Position");
		if (positionNode) {
			gameObjectConfig->SetPosition(Vector3Parser::Parse(positionNode));
		}

		pugi::xml_node colorNode = gameObjectNode.child("Color");
		if (colorNode) {
			gameObjectConfig->SetColor(Vector3Parser::Parse(colorNode));
		}

		return gameObjectConfig;
	}
}