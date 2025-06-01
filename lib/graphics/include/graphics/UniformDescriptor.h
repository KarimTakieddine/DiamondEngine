#pragma once

#include <GL/glew.h>

#include <string>

namespace diamond_engine
{
	struct UniformDescriptor
	{
		enum class Type : GLubyte
		{
			UNKNOWN		= 0,
			SCALAR		= 1,
			VECTOR_2	= 2,
			VECTOR_3	= 3,
			MATRIX_2	= 4,
			MATRIX_3	= 5,
			MATRIX_4	= 6
		};

		std::string name{ "Unnamed" };
		GLsizei count	{ 0 };
		Type type		{ Type::UNKNOWN };
	};
}
