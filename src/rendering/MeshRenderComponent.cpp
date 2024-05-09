#include "MeshRenderComponent.h"
#include "RenderDrawCall.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	std::vector<RenderUpload> MeshRenderComponent::getUploads() const
	{
		return { };
	}

	EngineStatus MeshRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
		{
			return { "MeshRenderComponent::bindToShaderProgramFailed. Shader program was null", true };
		}

		m_vertexAttributeLocation		= shaderProgram->GetAttribute("position");
		m_colorAttributeLocation		= shaderProgram->GetAttribute("color");
		m_textureCoordAttributeLocation = shaderProgram->GetAttribute("textureCoordinate");

		return { };
	}

	EngineStatus MeshRenderComponent::onDrawCallRegistered(RenderDrawCall* renderDrawCall)
	{
		if (!renderDrawCall)
		{
			return { "MeshRenderComponent::onDrawCallRegistered failed. RendererDrawCall was null", true };
		}

		m_renderDrawCall = renderDrawCall;

		setMesh(m_mesh);	

		return { };
	}

	EngineStatus MeshRenderComponent::onRenderObjectAllocated(RenderObject* renderObject)
	{
		return { };
	}

	void MeshRenderComponent::setMesh(Mesh* mesh)
	{
		m_mesh = mesh;

		if (!m_renderDrawCall)
		{
			return;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->getVertexBufferObject());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh->getElementBufferObject());
		glBufferData(GL_ARRAY_BUFFER, m_mesh->GetVertices().size() * sizeof(Vertex), m_mesh->GetVertices().data(), m_drawMode);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh->GetTriangles().size() * sizeof(GLuint), m_mesh->GetTriangles().data(), m_drawMode);
		m_renderDrawCall->elementCount = m_mesh->GetTriangles().size();

		glEnableVertexAttribArray(m_vertexAttributeLocation);
		glVertexAttribPointer(m_vertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		glEnableVertexAttribArray(m_colorAttributeLocation);
		glVertexAttribPointer(m_colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

		glEnableVertexAttribArray(m_textureCoordAttributeLocation);
		glVertexAttribPointer(m_textureCoordAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(glm::vec3)));
	}

	void MeshRenderComponent::setDrawMode(GLenum drawMode)
	{
		if (m_renderDrawCall)
		{
			m_renderDrawCall->drawMode = drawMode;
		}

		m_drawMode = drawMode;
	}

	GLenum MeshRenderComponent::getDrawMode() const
	{
		return m_drawMode;
	}
}