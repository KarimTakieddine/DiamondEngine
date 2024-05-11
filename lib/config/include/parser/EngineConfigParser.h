#pragma once

#include "EngineConfig.h"

namespace pugi { class xml_node; class xml_document; }

namespace diamond_engine {
class EngineConfigParser {
public:
	static EngineConfig Parse(const pugi::xml_node& engineConfigNode);

	static EngineConfig Parse(const pugi::xml_document& engineDocument);

	static EngineConfig ParseFromFile(const std::string& file);
};
}
