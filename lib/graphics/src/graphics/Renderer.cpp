#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "DrawCall.h"
#include "GLAllocator.h"
#include "IRenderComponent.h"
#include "SharedMeshStore.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "VertexState.h"

namespace diamond_engine
{
	Renderer::Renderer(MeshType meshType, GLenum drawMode, const std::shared_ptr<ShaderProgram>& shaderProgram) : m_drawMode(drawMode), m_shaderProgram(shaderProgram)
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

	void Renderer::render(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		for (const auto& renderComponent : renderComponents)
		{
			renderComponent->uploadGraphicsMemory(memoryPool);
		}

		const DrawCall* drawCall = reinterpret_cast<const DrawCall*>(memoryPool->peek());
		glBindTexture(GL_TEXTURE_2D, drawCall->texture);
		glDrawElements(drawCall->drawMode, drawCall->elementCount, GL_UNSIGNED_INT, nullptr);

		memoryPool->advanceSeek(sizeof(DrawCall));
	}

	void Renderer::uploadMeshData(GLenum drawType, const VertexState* vertexState, const std::vector<VertexAttribute>& vertexAttributes)
	{
		glBindVertexArray(vertexState->vertexArrayObject);

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

	const std::shared_ptr<ShaderProgram>& Renderer::getShaderProgram() const
	{
		return m_shaderProgram;
	}

	EngineStatus Renderer::allocateGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		for (const auto& renderComponent : renderComponents)
		{
			const EngineStatus requestStatus = renderComponent->requestGraphicsMemory(memoryPool);
		
			if (!requestStatus)
			{
				return requestStatus;
			}
		}

		DrawCall* drawCall = memoryPool->requestMemory<DrawCall>();

		if (!drawCall)
		{
			return { "Failed to allocate draw call from memory pool", true };
		}

		for (const auto& renderComponent : renderComponents)
		{
			const EngineStatus handleStatus = renderComponent->onDrawCallAllocated(drawCall);

			if (!handleStatus)
			{
				return handleStatus;
			}
		}

		drawCall->drawMode		= m_drawMode;
		drawCall->elementCount	= m_sharedMesh->GetTriangles().size();

		return { };
	}

	EngineStatus Renderer::bindToShaderProgram(const RenderComponentList& renderComponents)
	{
		for (const auto& renderComponent : renderComponents)
		{
			const EngineStatus bindStatus = renderComponent->bindToShaderProgram(m_shaderProgram);
		
			if (!bindStatus)
				return bindStatus;
		}

		return { };
	}

	EngineStatus Renderer::releaseGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		memoryPool->freeMemory(sizeof(DrawCall));

		for (auto renderComponentIt = renderComponents.rbegin(); renderComponentIt != renderComponents.rend(); ++renderComponentIt)
		{
			const EngineStatus releaseStatus = (*renderComponentIt)->releaseGraphicsMemory(memoryPool);

			if (!releaseStatus)
			{
				return releaseStatus;
			}
		}

		return { };
	}
}