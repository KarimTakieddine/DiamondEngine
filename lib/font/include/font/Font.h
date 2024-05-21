#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct Font
	{
		GLsizei width{ 0 };
		GLsizei height{ 0 };
		GLubyte symbol{ '\0' };
	};
}
