#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct Font
	{
		GLsizei row		{ 0 };
		GLsizei column	{ 0 };
		GLubyte symbol	{ '\0' };
	};
}
