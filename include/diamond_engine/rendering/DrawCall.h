#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct DrawCall
	{
		GLsizei elementCount{ 0 };
		GLenum drawMode{ GL_TRIANGLES };
	};
}
