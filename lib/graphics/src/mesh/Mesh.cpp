#include "Mesh.h"

namespace diamond_engine {
	Mesh::Mesh(const MeshData& data) : m_data(data) { }

	void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
		m_data.vertices = vertices;
	}

	const std::vector<Vertex>& Mesh::GetVertices() const {
		return m_data.vertices;
	}

	std::vector<Vertex>& Mesh::vertices() {
		return m_data.vertices;
	}

	void Mesh::SetTriangles(const std::vector<GLuint>& triangles) {
		m_data.triangles = triangles;
	}

	std::vector<GLuint>& Mesh::triangles() {
		return m_data.triangles;
	}

	const std::vector<GLuint>& Mesh::GetTriangles() const {
		return m_data.triangles;
	}

	GLuint Mesh::getVertexBufferObject() const
	{
		return m_vertexBufferObject;
	}

	void Mesh::setVertexBufferObject(GLuint vertexBufferObject)
	{
		m_vertexBufferObject = vertexBufferObject;
	}

	GLuint Mesh::getElementBufferObject() const
	{
		return m_elementBufferObject;
	}

	void Mesh::setElementBufferObject(GLuint elementBufferObject)
	{
		m_elementBufferObject = elementBufferObject;
	}
}