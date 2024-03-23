#pragma once

#include <vector>

#include "SoundMetadata.h"

namespace pugi { class xml_node; class xml_document; }

namespace diamond_engine
{
	using SoundMetadataList = std::vector<SoundMetadata>;

	class AudioMetadataParser
	{
	public:
		static SoundMetadataList parse(const std::string& file);
		static SoundMetadataList parse(const pugi::xml_document& document);
		static SoundMetadata parse(const pugi::xml_node& node);
	};
}
