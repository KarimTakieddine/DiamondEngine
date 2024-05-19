#pragma once

#include "SpriteSheetConfig.h"

namespace pugi { class xml_node; class xml_document; }

namespace diamond_engine
{
	using SpriteSheetConfigList = std::vector<SpriteSheetConfig>;

	class SpriteSheetConfigParser
	{
	public:
		static SpriteSheetConfig Parse(const pugi::xml_node& node);
		static SpriteSheetConfigList Parse(const pugi::xml_document& document);
		static SpriteSheetConfigList Parse(const std::string& file);
	};
}
