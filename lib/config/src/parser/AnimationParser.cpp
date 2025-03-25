#include <pugixml.hpp>

#include "SpriteAnimationParser.h"

namespace diamond_engine
{
	/* static */ SpriteAnimation SpriteAnimationParser::Parse(const pugi::xml_node& node)
	{
		SpriteAnimation result;

		pugi::xml_attribute nameAttribute = node.attribute("name");
		if (nameAttribute)
		{
			result.name = nameAttribute.as_string(result.name.c_str());
		}

		pugi::xml_attribute durationAttribute = node.attribute("duration");
		if (durationAttribute)
		{
			result.durationS = durationAttribute.as_float(result.durationS);
		}

		pugi::xml_attribute delayBetweenFramesAttribute = node.attribute("interFrameDelay");
		if (delayBetweenFramesAttribute)
		{
			result.interFrameDelayS = delayBetweenFramesAttribute.as_float(result.interFrameDelayS);
		}

		return result;
	}
}