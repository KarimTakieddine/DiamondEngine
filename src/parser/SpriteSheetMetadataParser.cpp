#include <stdexcept>

#include <pugixml.hpp>

#include "FrameMetadataParser.h"
#include "SpriteSheetMetadataParser.h"

namespace diamond_engine
{
	/* static */ SpriteSheetMetadata SpriteSheetMetadataParser::Parse(const pugi::xml_node& node)
	{
		SpriteSheetMetadata result;

		pugi::xml_attribute nameAttribute = node.attribute("name");

		if (!nameAttribute)
		{
			throw std::runtime_error("No sprite sheet \"name\" attribute was set");
		}

		result.setName(nameAttribute.as_string(result.getName().c_str()));

		pugi::xml_attribute spriteSheetAttribute = node.attribute("spriteSheet");

		if (!spriteSheetAttribute)
		{
			throw std::runtime_error("No sprite sheet \"spriteSheet\" attribute was set");
		}

		result.setSpriteSheet(spriteSheetAttribute.as_string(result.getSpriteSheet().c_str()));

		for (const auto& frameNode : node.children("Frame"))
		{
			result.addFrame(FrameMetadataParser::Parse(frameNode));
		}

		return result;
	}

	/* static */ SpriteSheetList SpriteSheetMetadataParser::Parse(const pugi::xml_document& document)
	{
		SpriteSheetList result;

		pugi::xml_node rootNode = document.root().first_child();

		if (!rootNode)
		{
			throw std::runtime_error("No root node has been set on SpriteSheet list document");
		}

		for (const auto& spriteSheetNode : rootNode.children("SpriteSheet"))
		{
			result.push_back(Parse(spriteSheetNode));
		}

		return result;
	}

	/* static */ SpriteSheetList SpriteSheetMetadataParser::Parse(const std::string& file)
	{
		pugi::xml_document document;
		pugi::xml_parse_result parseResult = document.load_file(file.c_str());
		if (!parseResult)
		{
			throw std::runtime_error("Failed to parse SpriteSheet list document. Error was: " + std::string(parseResult.description()));
		}
		return Parse(document);
	}
}