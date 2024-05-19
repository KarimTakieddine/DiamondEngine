#pragma once

#include "Animation.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class AnimationParser
	{
	public:
		static Animation Parse(const pugi::xml_node& node);
	};
}
