#pragma once

#include "SpriteSheetMetadata.h"

namespace pugi { class xml_node; class xml_document; }

namespace diamond_engine
{
	using SpriteSheetList = std::vector<SpriteSheetMetadata>;

	class SpriteSheetMetadataParser
	{
	public:
		static SpriteSheetMetadata Parse(const pugi::xml_node& node);
		static SpriteSheetList Parse(const pugi::xml_document& document);
		static SpriteSheetList Parse(const std::string& file);
	};
}
