#pragma once

#include "SpriteAnimation.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class SpriteAnimationParser
	{
	public:
		static SpriteAnimation Parse(const pugi::xml_node& node);
	};
}
