#pragma once

#include "FrameMetadata.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class FrameMetadataParser
	{
	public:
		static FrameMetadata Parse(const pugi::xml_node& node);
	};
}
