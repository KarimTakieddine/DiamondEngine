#include <sstream>

#include <pugixml.hpp>

#include "FrameConfigParser.h"

namespace
{
	using diamond_engine::FrameType;

	static const std::unordered_map<std::string, FrameType> kStringToFrameType =
	{
		{ "NONE",				FrameType::NONE					},
		{ "LINKABLE",			FrameType::LINKABLE				},
		{ "CANCELLABLE",		FrameType::CANCELLABLE			},
		{ "SUPER_CANCELLABLE",	FrameType::SUPER_CANCELLABLE	},
		{ "ACTIVE",				FrameType::ACTIVE				},
		{ "RECOVERY",			FrameType::RECOVERY				},
		{ "HIT_STUN",			FrameType::HIT_STUN				},
		{ "BLOCK_STUN",			FrameType::BLOCK_STUN			},
		{ "STARTUP",			FrameType::STARTUP				}
	};
}

namespace diamond_engine
{
	/* static */ FrameConfig FrameConfigParser::Parse(const pugi::xml_node& node)
	{
		FrameConfig result;

		pugi::xml_attribute xAttribute = node.attribute("x");
		if (xAttribute)
			result.x = xAttribute.as_uint(result.x);

		pugi::xml_attribute yAttribute = node.attribute("y");
		if (yAttribute)
			result.y = yAttribute.as_uint(result.y);

		pugi::xml_attribute widthAttribute = node.attribute("width");
		if (widthAttribute)
			result.width = widthAttribute.as_uint(result.width);

		pugi::xml_attribute heightAttribute = node.attribute("height");
		if (heightAttribute)
			result.height = heightAttribute.as_uint(result.height);

		pugi::xml_attribute frameTypeAttribute = node.attribute("type");
		if (frameTypeAttribute)
			result.type = parseFrameType(frameTypeAttribute.as_string());

		return result;
	}

	/* static */ FrameType FrameConfigParser::parseFrameType(const std::string& frameType, FrameType def /* = FrameType::NONE */)
	{
		if (frameType.empty())
		{
			return def;
		}

		std::stringstream stream(frameType);

		FrameType result = FrameType::NONE;

		std::string section;
		while (std::getline(stream, section, '|'))
		{
			if (section.empty())
			{
				continue;
			}

			const auto frameTypeIt = ::kStringToFrameType.find(section);

			if (frameTypeIt == ::kStringToFrameType.cend())
			{
				continue;
			}

			result |= frameTypeIt->second;
		}

		return result;
	}
}