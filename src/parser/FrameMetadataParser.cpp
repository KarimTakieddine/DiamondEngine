#include <pugixml.hpp>

#include "FrameMetadataParser.h"

namespace diamond_engine
{
	/* static */ FrameMetadata FrameMetadataParser::Parse(const pugi::xml_node& node)
	{
		FrameMetadata result;

		pugi::xml_attribute xAttribute = node.attribute("x");
		if (xAttribute)
		{
			result.x = xAttribute.as_uint(result.x);
		}

		pugi::xml_attribute yAttribute = node.attribute("y");
		if (yAttribute)
		{
			result.y = yAttribute.as_uint(result.y);
		}

		pugi::xml_attribute widthAttribute = node.attribute("width");
		if (widthAttribute)
		{
			result.width = widthAttribute.as_uint(result.width);
		}

		pugi::xml_attribute heightAttribute = node.attribute("height");
		if (heightAttribute)
		{
			result.height = heightAttribute.as_uint(result.height);
		}

		return result;
	}
}