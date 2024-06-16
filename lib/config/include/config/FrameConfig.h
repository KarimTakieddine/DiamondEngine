#pragma once

#include "FrameType.h"

namespace diamond_engine
{
	struct FrameConfig
	{
		GLuint x		{ 0 };
		GLuint y		{ 0 };
		GLuint width	{ 0 };
		GLuint height	{ 0 };
		FrameType type	{ FrameType::NONE };
	};
}
