#include <pugixml.hpp>

#include "SizeParser.h"

namespace diamond_engine {
	/* static */ Size SizeParser::Parse(const pugi::xml_node& sizeNode) {
		Size def = { };
		return {
			sizeNode.attribute("width").as_int(def.width),
			sizeNode.attribute("height").as_int(def.height) };
	}
}