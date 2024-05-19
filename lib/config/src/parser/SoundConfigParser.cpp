#include <stdexcept>

#include <pugixml.hpp>

#include "SoundConfigParser.h"

namespace diamond_engine
{
	/* static */ SoundConfigList SoundConfigParser::parse(const std::string& file)
	{
		pugi::xml_document document;
		pugi::xml_parse_result parse_result = document.load_file(file.c_str(), pugi::parse_default, pugi::encoding_utf8);

		if (!parse_result)
		{
			throw std::runtime_error("Failed to parse audio metadata. Error was: " + std::string(parse_result.description()));
		}

		return parse(document);
	}

	/* static */ SoundConfigList SoundConfigParser::parse(const pugi::xml_document& document)
	{
		SoundConfigList result;

		const auto& rootNode = document.root().first_child();

		for (const auto& audioMetadataNode : rootNode.children("Sound"))
		{
			result.push_back(parse(audioMetadataNode));
		}

		return result;
	}

	/* static */ SoundConfig SoundConfigParser::parse(const pugi::xml_node& node)
	{
		SoundConfig result;

		pugi::xml_attribute nameAttribute = node.attribute("name");

		if (!nameAttribute)
		{
			throw std::runtime_error("<Sound/> node must have a \"name\" attribute set");
		}

		result.name = nameAttribute.as_string(result.name.c_str());

		pugi::xml_attribute pathAttribute = node.attribute("path");

		if (!pathAttribute)
		{
			throw std::runtime_error("<Sound/> node must have a \"path\" attribute set");
		}

		result.path = pathAttribute.as_string();

		pugi::xml_attribute defaultVolumeAttribute = node.attribute("defaultVolume");

		if (defaultVolumeAttribute)
		{
			result.defaultVolume = defaultVolumeAttribute.as_uint(result.defaultVolume);
		}

		pugi::xml_attribute preloadAttribute = node.attribute("preload");

		if (preloadAttribute)
		{
			result.preload = preloadAttribute.as_bool(result.preload);
		}

		return result;
	}
}