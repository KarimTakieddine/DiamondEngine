#pragma once

#include <vector>

#include "SoundConfig.h"

namespace pugi { class xml_node; class xml_document; }

namespace diamond_engine
{
	using SoundConfigList = std::vector<SoundConfig>;

	class SoundConfigParser
	{
	public:
		static SoundConfigList parse(const std::string& file);
		static SoundConfigList parse(const pugi::xml_document& document);
		static SoundConfig parse(const pugi::xml_node& node);
	};
}
