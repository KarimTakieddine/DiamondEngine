#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "RenderSequence.h"

namespace diamond_engine {
	void RenderSequence::SetShaderProgram(const SharedShaderProgram& shaderProgram) {
		if (!shaderProgram) {
			throw std::runtime_error("Attempt to set null SharedShaderProgram instance on renderable sequence");
		}

		m_shaderProgram = shaderProgram;
	}

	void RenderSequence::SetObjectAllocator(const SharedObjectAllocator& objectAllocator) {
		if (!objectAllocator) {
			throw std::runtime_error("Attempt to set null SharedObjectAllocator instance on renderable sequence");
		}

		m_objectAllocator = objectAllocator;
	}

	void RenderSequence::SetVertexArrayAllocator(const std::shared_ptr<GLAllocator>& vertexArrayAllocator) {
		// TODO: Throw here?

		m_vertexArrayAllocator = vertexArrayAllocator;
	}

	void RenderSequence::SetTextureLoader(const std::shared_ptr<TextureLoader>& textureLoader) {
		if (!textureLoader) {
			throw std::runtime_error("Attempt to set null TextureLoader instance on renderable sequence");
		}
		m_textureLoader = textureLoader;
	}

	void RenderSequence::SetRenderDescriptor(const RenderDescriptor& renderDescriptor) {
		m_renderDescriptor = renderDescriptor;
	}

	const RenderSequence::RenderDescriptor& RenderSequence::GetRenderDescriptor() const {
		return m_renderDescriptor;
	}

	void RenderSequence::AddGameObject(std::unique_ptr<GameObject> gameObject, const MaterialConfig& materialConfig, const glm::vec3& position) {
		RenderableObject* renderableObject = m_objectAllocator->Get();

		if (!renderableObject) {
			throw std::runtime_error("Failed to allocate RenderableObject instance for game object");
		}

		renderableObject->vertexArrayObject					= m_vertexArrayAllocator->Get();
		renderableObject->objectTranslationUniformLocation	= m_shaderProgram->GetUniform(m_renderDescriptor.objectTranslationUniform);
		renderableObject->objectRotationUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.objectRotationUniform);
		renderableObject->objectScaleUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.objectScaleUniform);
		renderableObject->colorUniformLocation				= m_shaderProgram->GetUniform(m_renderDescriptor.colorUniform);
		renderableObject->textureOffsetUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.textureOffsetUniform);

		renderableObject->transform.SetPosition(position);
		renderableObject->material.SetColor(materialConfig.GetColor());
		renderableObject->material.SetTextureOffset(materialConfig.GetTextureOffset());
		renderableObject->material.SetTexture(m_textureLoader->GetTextureIndex(materialConfig.GetTextureName()));

		gameObject->SetRenderableObject(renderableObject);
		gameObject->BindToShaderProgram(m_shaderProgram);
		
		glUseProgram(m_shaderProgram->GetObject());
		gameObject->OnAddedToScene();

		m_gameObjects.push_back(std::move(gameObject));
	}

	void RenderSequence::SetCamera(const std::shared_ptr<Camera> camera) {
		if (!camera) {
			throw std::runtime_error("Attempt to set null Camera on RenderSequence instance");
		}

		m_projectionUniformLocation			= m_shaderProgram->GetUniform(m_renderDescriptor.projectionUniform);
		m_cameraTranslationUniformLocation	= m_shaderProgram->GetUniform(m_renderDescriptor.cameraTranslationUniform);
		m_cameraRotationUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.cameraRotationUniform);
		m_cameraScaleUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.cameraScaleUniform);
		m_cameraViewUniformLocation			= m_shaderProgram->GetUniform(m_renderDescriptor.cameraViewUniform);

		m_camera = camera;
	}

	void RenderSequence::Update(GLfloat deltaTime)
	{
		for (const auto& gameObject : m_gameObjects) {
			gameObject->Update(deltaTime);
		}
	}

	void diamond_engine::RenderSequence::Render()
	{
		glUseProgram(m_shaderProgram->GetObject());

		const glm::mat4& projectionMatrix = m_camera->GetProjection();
		const glm::mat4& cameraTranslationMatrix = m_camera->GetTransform().GetTranslation();
		const glm::mat4& cameraRotationMatrix = m_camera->GetTransform().GetRotation();
		const glm::mat4& cameraScaleMatrix = m_camera->GetTransform().GetScale();
		const glm::mat4& cameraViewMatrix = m_camera->GetView();

		glUniformMatrix4fv(m_projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(m_cameraTranslationUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraTranslationMatrix));
		glUniformMatrix4fv(m_cameraRotationUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraRotationMatrix));
		glUniformMatrix4fv(m_cameraScaleUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraScaleMatrix));
		glUniformMatrix4fv(m_cameraViewUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

		for (const auto& gameObject : m_gameObjects) {
			RenderableObject* renderableObject = gameObject->GetRenderableObject();

			const glm::mat4& objectTranslationMatrix = renderableObject->transform.GetTranslation();
			glUniformMatrix4fv(renderableObject->objectTranslationUniformLocation, 1, GL_FALSE, glm::value_ptr(objectTranslationMatrix));

			const glm::mat4& objectRotationMatrix = renderableObject->transform.GetRotation();
			glUniformMatrix4fv(renderableObject->objectRotationUniformLocation, 1, GL_FALSE, glm::value_ptr(objectRotationMatrix));

			const glm::mat4& objectScaleMatrix = renderableObject->transform.GetScale();
			glUniformMatrix4fv(renderableObject->objectScaleUniformLocation, 1, GL_FALSE, glm::value_ptr(objectScaleMatrix));

			const glm::vec3& materialColor = renderableObject->material.GetColor();
			glUniform3f(renderableObject->colorUniformLocation, materialColor.r, materialColor.g, materialColor.b);

			const glm::vec2& materialTextureOffset = renderableObject->material.GetTextureOffset();
			glUniform2f(renderableObject->textureOffsetUniformLocation, materialTextureOffset.x, materialTextureOffset.y);

			const GLint texture = renderableObject->material.GetTexture();
			if ((!m_boundTexture.has_value() || (m_boundTexture.value() != texture)) && texture != 0)
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				m_boundTexture = texture;
			}

			glBindVertexArray(renderableObject->vertexArrayObject);
			glDrawElements(renderableObject->drawCall.drawMode, renderableObject->drawCall.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}