#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct DrawCall
	{
		GLuint texture{ 0 };
		GLenum drawMode{ GL_TRIANGLES };
		GLsizei elementCount{ 0 };
		GLubyte padding[4];
	};
}
