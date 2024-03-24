#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct FrameMetadata
	{
		GLuint x{ 0 };
		GLuint y{ 0 };
		GLuint width{ 0 };
		GLuint height{ 0 };
	};
}
