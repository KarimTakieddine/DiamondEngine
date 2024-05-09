#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct RenderDrawCall
	{
		GLenum drawMode{ GL_TRIANGLES };
		GLuint texture{ 0 };
		GLsizei elementCount{ 0 };
	};
}
