#include <stdexcept>

#include <pugixml.hpp>

#include "Collider2DConfigParser.h"
#include "GameObjectConfigParser.h"
#include "MaterialConfigParser.h"
#include "MeshRendererConfigParser.h"
#include "RotateBehaviourConfigParser.h"
#include "ScrollingBackgroundBehaviourConfigParser.h"
#include "Vector3Parser.h"

namespace diamond_engine {
	static const std::unordered_map<std::string, GameObjectType> kStringToGameObjectType = {
		{ "sprite", GameObjectType::SPRITE }
	};

	/* static */ std::unordered_map<std::string, GameObjectConfigParser::ComponentParseFunc> GameObjectConfigParser::StringToComponentMap = {
		{ "MeshRenderer",	&MeshRendererConfigParser::Parse },
		{ "Collider2D",		&Collider2DConfigParser::Parse }
	};

	/* static */ std::unordered_map<std::string, GameObjectConfigParser::BehaviourParseFunc> GameObjectConfigParser::StringToBehaviourMap = {
		{ "RotateBehaviour",				&RotateBehaviourConfigParser::Parse },
		{ "ScrollingBackgroundBehaviour",	&ScrollingBackgroundBehaviourConfigParser::Parse }
	};

	/* static */ std::unique_ptr<GameObjectConfig> GameObjectConfigParser::Parse(const pugi::xml_node& gameObjectNode) {
		if (!gameObjectNode) {
			throw std::runtime_error("No <GameObject/> node supplied to GameObjectConfigParser::Parse()");
		}

		// TODO: Ensure no 2 gameobjects have the same name

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

		pugi::xml_attribute nameAttribute = gameObjectNode.attribute("name");
		if (nameAttribute)
		{
			gameObjectConfig->setName(nameAttribute.as_string());
		}

		pugi::xml_node componentsNode = gameObjectNode.child("Components");
		if (componentsNode) {
			for (pugi::xml_node componentNode : componentsNode.children()) {
				const std::string componentNameString(componentNode.name());

				auto componentParseFuncIt = StringToComponentMap.find(componentNameString);
				if (componentParseFuncIt == StringToComponentMap.end()) {
					throw std::runtime_error("Unknown component name specified: \"" + componentNameString + "\". Did you forget to register the component type?");
				}

				gameObjectConfig->AddComponentConfig(componentParseFuncIt->second(componentNode));
			}
		}

		pugi::xml_node behavioursNode = gameObjectNode.child("Behaviours");
		if (behavioursNode) {
			for (pugi::xml_node behaviourNode : behavioursNode.children()) {
				const std::string behaviourNameString(behaviourNode.name());

				auto behaviourParseFuncIt = StringToBehaviourMap.find(behaviourNameString);
				if (behaviourParseFuncIt == StringToBehaviourMap.end()) {
					throw std::runtime_error("Unknown behaviour name specified: \"" + behaviourNameString + "\". Did you forget to register the behaviour type?");
				}

				gameObjectConfig->AddBehaviourConfig(behaviourParseFuncIt->second(behaviourNode));
			}
		}

		pugi::xml_node positionNode = gameObjectNode.child("Position");
		if (positionNode) {
			gameObjectConfig->SetPosition(Vector3Parser::Parse(positionNode));
		}

		pugi::xml_node scaleNode = gameObjectNode.child("Scale");
		if (scaleNode)
		{
			gameObjectConfig->SetScale(Vector3Parser::Parse(scaleNode));
		}
		else
		{
			gameObjectConfig->SetScale({ 1.0f, 1.0f, 1.0f });
		}

		pugi::xml_node materialNode = gameObjectNode.child("Material");
		if (materialNode) {
			gameObjectConfig->SetMaterialConfig(MaterialConfigParser::Parse(materialNode));
		}

		return gameObjectConfig;
	}

	/* static */ void GameObjectConfigParser::RegisterBehaviourConfig(const std::string& name, BehaviourParseFunc behaviourParseFunc) {
		StringToBehaviourMap.emplace(name, behaviourParseFunc);
	}
}