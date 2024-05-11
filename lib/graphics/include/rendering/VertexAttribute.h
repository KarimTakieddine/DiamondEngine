#pragma once

#include <string>

#include <GL/glew.h>

namespace diamond_engine
{
	struct VertexAttribute
	{
		std::string name{ "Unnamed" };
		size_t offset	{ 0 };
		GLsizei count	{ 0 };
		GLsizei stride	{ 0 };
		GLenum type		{ GL_TYPE };
	};
}
