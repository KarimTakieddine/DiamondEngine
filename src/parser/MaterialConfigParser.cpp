#include <stdexcept>

#include <pugixml.hpp>

#include "MaterialConfigParser.h"
#include "Vector2Parser.h"
#include "Vector3Parser.h"

namespace diamond_engine {
/* static */ MaterialConfig MaterialConfigParser::Parse(const pugi::xml_node& node) {
	MaterialConfig result = { };

	const pugi::xml_node textureNode = node.child("Texture");
	if (textureNode) {
		const pugi::xml_attribute textureNameAttribute = textureNode.attribute("name");

		if (!textureNameAttribute) {
			throw std::runtime_error("Could not find \"name\" attribute on <Texture/> node");
		}

		result.SetTextureName(textureNameAttribute.as_string());
	}

	const pugi::xml_node colorNode = node.child("Color");
	if (colorNode) {
		result.SetColor(Vector3Parser::Parse(colorNode));
	}

	const pugi::xml_node textureOffsetNode = node.child("TextureOffset");
	if (textureOffsetNode) {
		result.SetTextureOffset(Vector2Parser::Parse(textureOffsetNode));
	}

	return result;
}
}