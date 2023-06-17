#include <string>
#include <stdexcept>

#include <pugixml.hpp>

#include "ComponentType.h"
#include "GameObjectConfigParser.h"
#include "MeshRendererConfigParser.h"

namespace diamond_engine {
	static const std::unordered_map<std::string, GameObjectType> kStringToGameObjectType = {
		{ "sprite", GameObjectType::SPRITE }
	};

	static const std::unordered_map<std::string, ComponentType> kStringToComponentType = {
		{ "MeshRenderer", ComponentType::MeshRenderer }
	};

	std::unique_ptr<GameObjectConfig> GameObjectConfigParser::Parse(const pugi::xml_node& gameObjectNode) {
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
				const std::string componentTypeString(componentNode.name());

				auto componentTypeIt = kStringToComponentType.find(componentTypeString);
				if (componentTypeIt == kStringToComponentType.end()) {
					throw std::runtime_error("Unknown component type specified: " + componentTypeString);
				}

				switch (componentTypeIt->second)
				{
				case ComponentType::MeshRenderer: {
					MeshRendererConfigParser configParser;
					gameObjectConfig->AddComponentConfig(configParser.Parse(componentNode));
				}
				default:
					break;
				}
			}
		}

		return gameObjectConfig;
	}
}