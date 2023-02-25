#pragma once

#include <vector>

#include <GL/glew.h>

#include "Vertex.h"

namespace diamond_engine {
class Mesh {
public:
	Mesh(
		const std::vector<Vertex>& vertices,
		const std::vector<GLuint>& triangles);

	void SetVertices(const std::vector<Vertex>& vertices);

	const std::vector<Vertex>& GetVertices() const;

	std::vector<Vertex>& vertices();

	void SetTriangles(const std::vector<GLuint>& triangles);

	std::vector<GLuint>& triangles();

	const std::vector<GLuint>& GetTriangles() const;

private:
	std::vector<Vertex> m_vertices{ };
	std::vector<GLuint> m_triangles{ };
};
}
