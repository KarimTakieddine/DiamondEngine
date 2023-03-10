#include "GameObject.h"
#include "MeshRenderer.h"

namespace diamond_engine {
	/* static */ const std::string MeshRenderer::kVertexAttributeLocation				= "position";
	/* static */ const std::string MeshRenderer::kColorAttributeLocation				= "color";
	/* static */ const std::string MeshRenderer::kTextureCoordinateAttributeLocation	= "textureCoordinate";

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

		GLint vertexAttributeLocation = glGetAttribLocation(m_gameObject->GetShaderProgram()->GetObject(), kVertexAttributeLocation.c_str());
		glEnableVertexAttribArray(vertexAttributeLocation);
		glVertexAttribPointer(vertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		GLint colorAttributeLocation = glGetAttribLocation(m_gameObject->GetShaderProgram()->GetObject(), kColorAttributeLocation.c_str());
		glEnableVertexAttribArray(colorAttributeLocation);
		glVertexAttribPointer(colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

		GLint textureCoordinateAttributeLocation = glGetAttribLocation(m_gameObject->GetShaderProgram()->GetObject(), kTextureCoordinateAttributeLocation.c_str());
		glEnableVertexAttribArray(textureCoordinateAttributeLocation);
		glVertexAttribPointer(textureCoordinateAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(glm::vec3)));

		if (m_material) {
			m_material->BindToContext();
		}
	}

	void MeshRenderer::Render() {
		glBindVertexArray(m_vertexArrayObject);

		if (m_mesh) {
			const size_t size = m_mesh->GetTriangles().size();

			glDrawElements(
				GetDrawMode(m_renderMode),
				m_mesh->GetTriangles().size(),
				GL_UNSIGNED_INT,
				0);
		}
	}

	void MeshRenderer::Update(GLfloat deltaTime) {
		if (m_material) {
			m_material->Update(deltaTime);
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

	void MeshRenderer::SetMaterial(const std::shared_ptr<Material>& material) {
		m_material = material;
	}

	const std::shared_ptr<Material>& MeshRenderer::GetMaterial() const {
		return m_material;
	}

	void MeshRenderer::OnSetGameObject() {
		if (m_vertexShader) {
			m_gameObject->GetShaderProgram()->AttachShader(m_vertexShader);
		}

		if (m_material) {
			m_material->SetGameObject(m_gameObject);
		}
	}

	void MeshRenderer::OnGameObjectAboutToBeUnset() {
		if (m_material) {
			m_material->UnsetGameObject();
		}

		if (m_vertexShader) {
			m_gameObject->GetShaderProgram()->DetachShader(m_vertexShader);
		}
	}
}