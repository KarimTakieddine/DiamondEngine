#include "Mesh.h"
#include "MeshRenderer.h"
#include "Shader.h"

namespace diamond_engine {
	/* static */ const std::string MeshRenderer::kVertexAttributeLocation	= "position";
	/* static */ const std::string MeshRenderer::kColorAttributeLocation	= "color";

	MeshRenderer::MeshRenderer(
		GLuint vertexArrayObject,
		GLuint vertexBufferObject,
		GLuint elementBufferObject) :
		m_vertexArrayObject(vertexArrayObject),
		m_vertexBufferObject(vertexBufferObject),
		m_elementBufferObject(elementBufferObject) { }

	void MeshRenderer::BindToContext() {
		glBindVertexArray(m_vertexArrayObject);

		if (m_mesh) {
			const std::vector<Vertex>& meshVertices = m_mesh->GetVertices();
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), meshVertices.data(), GetDrawType(m_meshType));

			const std::vector<GLuint>& meshTriangles = m_mesh->GetTriangles();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshTriangles.size() * sizeof(GLuint), meshTriangles.data(), GetDrawType(m_meshType));
		}
	}

	void MeshRenderer::Render() {
		glBindVertexArray(m_vertexArrayObject);

		if (m_mesh) {
			glDrawElements(
				GetDrawMode(m_renderMode),
				m_mesh->GetTriangles().size(),
				GL_UNSIGNED_INT,
				nullptr);
		}
	}

	void MeshRenderer::SetMeshType(MeshType meshType) {
		m_meshType = meshType;
	}

	void MeshRenderer::SetRenderMode(RenderMode renderMode) {
		m_renderMode = renderMode;
	}

	void MeshRenderer::SetMesh(const std::shared_ptr<Mesh>& mesh) {
		m_mesh = mesh;
	}

	void MeshRenderer::SetVertexShader(const std::shared_ptr<Shader>& vertexShader) {
		m_vertexShader = vertexShader;
	}
}