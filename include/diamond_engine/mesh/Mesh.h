#pragma once

#include "MeshData.h"

namespace diamond_engine {
class Mesh {
public:
	Mesh(const MeshData& data);

	void SetVertices(const std::vector<Vertex>& vertices);

	const std::vector<Vertex>& GetVertices() const;

	std::vector<Vertex>& vertices();

	void SetTriangles(const std::vector<GLuint>& triangles);

	std::vector<GLuint>& triangles();

	const std::vector<GLuint>& GetTriangles() const;

	GLuint getVertexBufferObject() const;

	void setVertexBufferObject(GLuint vertexBufferObject);

	GLuint getElementBufferObject() const;

	void setElementBufferObject(GLuint elementBufferObject);

private:
	MeshData m_data;
	GLuint m_vertexBufferObject{ 0 };
	GLuint m_elementBufferObject{ 0 };
};
}
