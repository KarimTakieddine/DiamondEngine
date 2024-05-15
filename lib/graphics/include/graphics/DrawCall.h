#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct DrawCall
	{
		GLuint texture{ 0 };
		GLsizei elementCount{ 0 };
	};
}
