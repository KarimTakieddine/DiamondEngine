#include "mesh/Mesh.h"

namespace diamond_engine {
	Mesh::Mesh(
		const std::vector<Vertex>& vertices,
		const std::vector<GLuint>& triangles) : m_vertices(vertices), m_triangles(triangles) { }
	
	void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
		m_vertices = vertices;
	}
	
	const std::vector<Vertex>& Mesh::GetVertices() const {
		return m_vertices;
	}

	std::vector<Vertex>& Mesh::vertices() {
		return m_vertices;
	}

	void Mesh::SetTriangles(const std::vector<GLuint>& triangles) {
		m_triangles = triangles;
	}

	std::vector<GLuint>& Mesh::triangles() {
		return m_triangles;
	}

	const std::vector<GLuint>& Mesh::GetTriangles() const {
		return m_triangles;
	}
}