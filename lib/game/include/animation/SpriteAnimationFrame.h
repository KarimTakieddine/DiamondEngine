#pragma once

#include "FrameType.h"

namespace diamond_engine
{
	struct SpriteAnimationFrame
	{
		GLint texture{ -1 };
		FrameType type{ FrameType::NONE };
	};
}
