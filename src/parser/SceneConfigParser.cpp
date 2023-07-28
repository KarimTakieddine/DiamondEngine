#include <stdexcept>

#include <pugixml.hpp>

#include "parser/GameObjectConfigParser.h"
#include "parser/SceneConfigParser.h"

namespace diamond_engine {
	/* static */ std::unique_ptr<SceneConfig> SceneConfigParser::Parse(const pugi::xml_document& sceneDocument) {
		std::unique_ptr<SceneConfig> result = std::make_unique<SceneConfig>();

		pugi::xml_node sceneNode = sceneDocument.root().first_child();

		pugi::xml_attribute maxObjectsAttribute = sceneNode.attribute("maxObjects");

		if (!maxObjectsAttribute) {
			throw std::runtime_error("No \"maxObjects\" attribute set on root <Scene/> node");
		}

		const int maxObjects = maxObjectsAttribute.as_int();

		if (maxObjects < 1) {
			throw std::runtime_error("Cannot build scene without at least one object");
		}

		result->SetMaxObjects(maxObjects);

		pugi::xml_node gameObjectsNode = sceneNode.child("GameObjects");

		if (!gameObjectsNode) {
			return result;
		}

		for (pugi::xml_node gameObjectNode : gameObjectsNode.children("GameObject")) {
			result->AddGameObjectConfig(GameObjectConfigParser::Parse(gameObjectNode));
		}
		
		return result;
	}
}