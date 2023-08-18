#include <pugixml.hpp>

#include "SizeParser.h"
#include "WindowConfigParser.h"

namespace diamond_engine {
	/* static */ WindowConfig WindowConfigParser::Parse(const pugi::xml_node& windowConfigNode) {
		WindowConfig def = { };

		return {
			windowConfigNode.attribute("title").as_string(def.GetTitle().c_str()),
			SizeParser::Parse(windowConfigNode) };
	}
}