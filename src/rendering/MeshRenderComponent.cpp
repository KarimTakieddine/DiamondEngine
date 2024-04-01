#include "GLAllocator.h"
#include "Mesh.h"
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

		if (!m_sharedBufferAllocator)
		{
			return { "MeshRenderComponent::onDrawCallRegistered failed. No shared buffer allocator was set", true };
		}

		m_vertexBufferObject	= m_sharedBufferAllocator->Get();
		m_elementBufferObject	= m_sharedBufferAllocator->Get();

		if (!m_sharedMesh)
		{
			return { "MeshRenderComponent::onDrawCallRegistered failed. No shared Mesh was set", true };
		}

		glBindVertexArray(renderDrawCall->vertexArrayObject);

		const std::vector<Vertex>& meshVertices = m_sharedMesh->GetVertices();
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), meshVertices.data(), m_drawMode);

		const std::vector<GLuint>& meshTriangles = m_sharedMesh->GetTriangles();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshTriangles.size() * sizeof(GLuint), meshTriangles.data(), m_drawMode);

		renderDrawCall->elementCount = meshTriangles.size();

		glEnableVertexAttribArray(m_vertexAttributeLocation);
		glVertexAttribPointer(m_vertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		glEnableVertexAttribArray(m_colorAttributeLocation);
		glVertexAttribPointer(m_colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(glm::vec3)));

		glEnableVertexAttribArray(m_textureCoordAttributeLocation);
		glVertexAttribPointer(m_textureCoordAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(2 * sizeof(glm::vec3)));

		return { };
	}

	EngineStatus MeshRenderComponent::onRenderObjectAllocated(RenderObject* renderObject)
	{
		return { };
	}

	void MeshRenderComponent::setSharedBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator)
	{
		m_sharedBufferAllocator = bufferAllocator;
	}

	void MeshRenderComponent::setSharedMesh(const std::shared_ptr<Mesh>& sharedMesh)
	{
		m_sharedMesh = sharedMesh;
	}

	void MeshRenderComponent::setDrawMode(GLenum drawMode)
	{
		m_drawMode = drawMode;
	}

	GLenum MeshRenderComponent::getDrawMode() const
	{
		return m_drawMode;
	}
}