#include <pugixml.hpp>

#include "WindowConfigParser.h"

namespace diamond_engine {
	/* static */ WindowConfig WindowConfigParser::Parse(const pugi::xml_node& windowConfigNode) {
		WindowConfig def = { };

		return {
			windowConfigNode.attribute("title").as_string(def.GetTitle().c_str()),
			{
				windowConfigNode.attribute("width").as_int(def.GetSize().width),
				windowConfigNode.attribute("height").as_int(def.GetSize().height)
			} };
	}
}