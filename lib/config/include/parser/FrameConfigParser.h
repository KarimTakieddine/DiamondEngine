#pragma once

#include "FrameConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class FrameConfigParser
	{
	public:
		static FrameConfig Parse(const pugi::xml_node& node);
	};
}
