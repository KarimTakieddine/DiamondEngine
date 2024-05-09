#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "GLAllocator.h"
#include "IRenderComponent.h"
#include "SharedMeshStore.h"
#include "Renderer.h"

namespace diamond_engine
{
	Renderer::Renderer(GLuint vertexArrayObject, MeshType meshType, GLenum drawMode, const std::shared_ptr<ShaderProgram>& shaderProgram) : m_vertexArrayObject(vertexArrayObject), m_drawMode(drawMode), m_shaderProgram(shaderProgram)
	{
		if (!m_shaderProgram)
		{
			throw std::runtime_error{ "Failed to create Renderer. No shader program was set" };
		}

		m_sharedMesh = SharedMeshStore::getInstance()->FindMesh(meshType);

		if (!m_sharedMesh)
		{
			throw std::runtime_error{ "Failed to create Renderer. No shared mesh was set" };
		}
	}

	void Renderer::clearRenderInstructions()
	{
		m_renderInstructions.clear();
	}

	void Renderer::registerRenderInstruction(const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents, RenderDrawCall* renderDrawCall)
	{
		renderDrawCall->drawMode		= m_drawMode;
		renderDrawCall->elementCount	= m_sharedMesh->GetTriangles().size();

		std::vector<RenderUpload> renderUploads;
		for (const auto& renderComponent : renderComponents)
		{
			EngineStatus bindStatus = renderComponent->bindToShaderProgram(m_shaderProgram);

			if (!bindStatus)
			{
				throw std::runtime_error(bindStatus.message);
			}

			const std::vector<RenderUpload> componentUploads = renderComponent->getUploads();
			renderUploads.insert(renderUploads.end(), componentUploads.begin(), componentUploads.end());

			EngineStatus drawCallStatus = renderComponent->onDrawCallRegistered(renderDrawCall);

			if (!drawCallStatus)
			{
				throw std::runtime_error(drawCallStatus.message);
			}
		}

		m_renderInstructions.push_back({ RenderUploadObject{ renderUploads }, *renderDrawCall });
	}

	const std::vector<RenderInstruction>& Renderer::getInstructions() const
	{
		return m_renderInstructions;
	}

	std::vector<RenderInstruction>& Renderer::getInstructions()
	{
		return m_renderInstructions;
	}

	void Renderer::render()
	{
		glBindVertexArray(m_vertexArrayObject);

		glUseProgram(m_shaderProgram->GetObject());

		for (const auto& cameraUpload : m_cameraUploads)
		{
			performUpload(cameraUpload);
		}

		for (const auto& renderInstruction : m_renderInstructions)
		{
			const RenderUploadObject& uploadObject = renderInstruction.uploadObject;
			for (const auto& renderUpload : uploadObject.getUploads())
			{
				performUpload(renderUpload);
			}

			const RenderDrawCall& drawCall = renderInstruction.drawCall;
			glBindTexture(GL_TEXTURE_2D, drawCall.texture);
			glDrawElements(drawCall.drawMode, drawCall.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}

	void Renderer::uploadMeshData(const std::vector<VertexAttribute>& vertexAttributes, GLenum drawType)
	{
		glBindVertexArray(m_vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_sharedMesh->getVertexBufferObject());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sharedMesh->getElementBufferObject());
		glBufferData(GL_ARRAY_BUFFER, m_sharedMesh->GetVertices().size() * sizeof(Vertex), m_sharedMesh->GetVertices().data(), drawType);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sharedMesh->GetTriangles().size() * sizeof(GLuint), m_sharedMesh->GetTriangles().data(), drawType);

		for (const auto& vertexAttribute : vertexAttributes)
		{
			const GLint attributeLocation = m_shaderProgram->GetAttribute(vertexAttribute.name);
			glEnableVertexAttribArray(attributeLocation);
			glVertexAttribPointer(
				attributeLocation,
				vertexAttribute.count,
				vertexAttribute.type,
				GL_FALSE,
				vertexAttribute.stride,
				reinterpret_cast<const void*>(vertexAttribute.offset));
		}
	}

	/* static */ void Renderer::performUpload(const RenderUpload& renderUpload)
	{
		switch (renderUpload.type)
		{
		case RenderUploadType::Matrix4:
			glUniformMatrix4fv(renderUpload.location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(renderUpload.data));
			break;
		case RenderUploadType::Vector3:
			glUniform3fv(renderUpload.location, 1, reinterpret_cast<const GLfloat*>(renderUpload.data));
			break;
		case RenderUploadType::Vector2:
			glUniform2fv(renderUpload.location, 1, reinterpret_cast<const GLfloat*>(renderUpload.data));
			// TODO: More cases, or a map!
			break;
		default:
			break;
		}
	}

	const std::shared_ptr<ShaderProgram>& Renderer::getShaderProgram() const
	{
		return m_shaderProgram;
	}

	void Renderer::setCamera(const std::shared_ptr<Camera>& camera)
	{
		if (camera)
		{
			m_cameraUploads = {
				{ &camera->GetProjection(),						RenderUploadType::Matrix4, m_shaderProgram->GetUniform("projection")},
				{ &camera->GetTransform().getLocalToWorld(),	RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraLocalToWorld")},
				{ &camera->GetTransform().getLocalRotation(),	RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraLocalRotation") },
				{ &camera->GetView(),							RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraView") }
			};
		}

		m_camera = camera;
	}
}