#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	struct VertexState
	{
		GLuint vertexArrayObject{ 0 };
		GLuint shaderProgram{ 0 };
	};
}
