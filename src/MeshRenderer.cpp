#include <stdexcept>

#include "GameObject.h"
#include "MeshRenderer.h"

namespace diamond_engine {
	/* static */ const std::string MeshRenderer::kVertexAttributeName	= "position";
	/* static */ const std::string MeshRenderer::kColorAttributeName	= "color";
	/* static */ const std::string MeshRenderer::kTextureCoordinateName	= "textureCoordinate";

	void MeshRenderer::BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) {
		m_vertexAttributeLocation	= shaderProgram->GetAttribute(kVertexAttributeName);
		m_colorAttributeLocation	= shaderProgram->GetAttribute(kColorAttributeName);
	}

	void MeshRenderer::OnAddedToScene() {
		glBindVertexArray(m_gameObject->GetRenderableObject()->vertexArrayObject);

		if (m_mesh) {
			const std::vector<Vertex>& meshVertices = m_mesh->GetVertices();
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), meshVertices.data(), m_drawMode);

			const std::vector<GLuint>& meshTriangles = m_mesh->GetTriangles();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshTriangles.size() * sizeof(GLuint), meshTriangles.data(), m_drawMode);

			m_gameObject->GetRenderableObject()->drawCall.drawMode		= m_drawMode;
			m_gameObject->GetRenderableObject()->drawCall.elementCount	= meshTriangles.size();
		}

		if (m_vertexAttributeLocation == -1) {
			throw std::runtime_error("Could not locate: " + kVertexAttributeName + " vertex attribute set on MeshRenderer instance");
		}

		glEnableVertexAttribArray(m_vertexAttributeLocation);
		glVertexAttribPointer(m_vertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		if (m_colorAttributeLocation == -1) {
			throw std::runtime_error("Could not locate: " + kColorAttributeName + " color attribute set on MeshRenderer instance");
		}

		glEnableVertexAttribArray(m_colorAttributeLocation);
		glVertexAttribPointer(m_colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));
	}

	void MeshRenderer::SetMesh(const std::shared_ptr<Mesh>& mesh) {
		m_mesh = mesh;
	}

	void MeshRenderer::SetVertexBufferObject(GLuint vertexBufferObject) {
		m_vertexBufferObject = vertexBufferObject;
	}

	void MeshRenderer::SetElementBufferObject(GLuint elementBufferObject) {
		m_elementBufferObject = elementBufferObject;
	}

	void MeshRenderer::SetVertexAttributeLocation(GLuint vertexAttributeLocation) {
		m_vertexAttributeLocation = vertexAttributeLocation;
	}

	void MeshRenderer::SetColorAttributeLocation(GLuint colorAttributeLocation) {
		m_colorAttributeLocation = colorAttributeLocation;
	}

	void MeshRenderer::SetDrawMode(GLenum drawMode) {
		m_drawMode = drawMode;
	}
}