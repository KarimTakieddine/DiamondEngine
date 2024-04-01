#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "GLAllocator.h"
#include "IRenderComponent.h"
#include "Renderer.h"

namespace diamond_engine
{
	Renderer::Renderer(const std::shared_ptr<ShaderProgram>& shaderProgram) : m_shaderProgram(shaderProgram)
	{
		if (!m_shaderProgram)
		{
			throw std::runtime_error{ "Failed to create Renderer. No shader program was set" };
		}
	}

	void Renderer::registerRenderInstruction(const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents, RenderDrawCall* renderDrawCall)
	{
		renderDrawCall->drawMode = m_drawMode;

		glUseProgram(m_shaderProgram->GetObject());

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
			glBindVertexArray(drawCall.vertexArrayObject);
			glDrawElements(drawCall.drawMode, drawCall.elementCount, GL_UNSIGNED_INT, nullptr);
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