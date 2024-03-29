#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct RenderDrawCall
	{
		GLuint vertexArrayObject{ 0 };
		GLuint texture{ 0 };
		GLsizei elementCount{ 0 };
		GLenum drawMode{ GL_TRIANGLES };
	};
}
