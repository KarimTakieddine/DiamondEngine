#pragma once

#include <vector>

#include <GL/glew.h>

#include "Vertex.h"

namespace diamond_engine
{
	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> triangles;
	};
}
