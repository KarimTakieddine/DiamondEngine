#include <pugixml.hpp>

#include "AnimationParser.h"

namespace diamond_engine
{
	/* static */ Animation AnimationParser::Parse(const pugi::xml_node& node)
	{
		Animation result;

		pugi::xml_attribute nameAttribute = node.attribute("name");
		if (nameAttribute)
		{
			result.name = nameAttribute.as_string(result.name.c_str());
		}

		pugi::xml_attribute durationAttribute = node.attribute("duration");
		if (durationAttribute)
		{
			result.duration = durationAttribute.as_float(result.duration);
		}

		pugi::xml_attribute delayBetweenFramesAttribute = node.attribute("delayBetweenFrames");
		if (delayBetweenFramesAttribute)
		{
			result.delayBetweenFrames = delayBetweenFramesAttribute.as_float(result.delayBetweenFrames);
		}

		return result;
	}
}