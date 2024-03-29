#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Renderer.h"

namespace diamond_engine
{
	Renderer::Renderer(const std::shared_ptr<ShaderProgram>& shaderProgram) : m_shaderProgram(shaderProgram)
	{
		if (!m_shaderProgram)
		{
			throw std::runtime_error{ "Failed to create Renderer.No shader program was set" };
		}
	}

	void Renderer::registerUploadObject(const std::vector<RenderUpload>& uploads)
	{
		RenderUploadObject renderObject;
		for (const auto& upload : uploads)
		{
			renderObject.registerUpload(upload);
		}

		m_renderInstructions.push_back({ renderObject, { } });
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

		// TODO: Find somewhere nicer to store the associated memory
		if (m_camera)
		{
			const auto& cameraProjection = m_camera->GetProjection();
			const auto& cameraView	= m_camera->GetView();
			const auto&cameraTransform = m_camera->GetTransform();

			const glm::mat4 cameraTranslation{ 1.0f };
			const glm::mat4 cameraRotation{ 1.0f };
			const glm::mat4 cameraScale{ 1.0f };

			performUpload({ glm::value_ptr(cameraProjection),	RenderUploadType::Matrix4, m_shaderProgram->GetUniform("projection") });
			performUpload({ glm::value_ptr(cameraView),			RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraView") });
			performUpload({ glm::value_ptr(cameraTranslation),	RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraTranslation") });
			performUpload({ glm::value_ptr(cameraRotation),		RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraRotation") });
			performUpload({ glm::value_ptr(cameraScale),		RenderUploadType::Matrix4, m_shaderProgram->GetUniform("cameraScale") });
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
		m_camera = camera;
	}
}