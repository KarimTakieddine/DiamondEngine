#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct Font
	{
		GLsizei width	{ 0 };
		GLsizei height	{ 0 };
		GLsizei row		{ 0 };
		GLsizei column	{ 0 };
		GLubyte symbol	{ '\0' };
	};
}
