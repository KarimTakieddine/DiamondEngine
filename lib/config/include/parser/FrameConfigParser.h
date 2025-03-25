#pragma once

#include <string>
#include <unordered_map>

#include "FrameConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class FrameConfigParser
	{
	public:
		static FrameConfig Parse(const pugi::xml_node& node);

	private:
		static FrameType parseFrameType(const std::string& frameType, FrameType def = FrameType::NONE);
	};
}
