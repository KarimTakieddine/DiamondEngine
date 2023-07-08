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

	void RenderSequence::SetRenderDescriptor(const RenderDescriptor& renderDescriptor) {
		m_renderDescriptor = renderDescriptor;
	}

	const RenderSequence::RenderDescriptor& RenderSequence::GetRenderDescriptor() const {
		return m_renderDescriptor;
	}

	void RenderSequence::AddGameObject(std::unique_ptr<GameObject> gameObject, const glm::vec3& position, const glm::vec3& color) {
		RenderableObject* renderableObject = m_objectAllocator->Get();

		if (!renderableObject) {
			throw std::runtime_error("Failed to allocate RenderableObject instance for game object");
		}

		renderableObject->vertexArrayObject			= m_vertexArrayAllocator->Get();
		renderableObject->transformUniformLocation	= m_shaderProgram->GetUniform(m_renderDescriptor.modelUniform);
		renderableObject->colorUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.colorUniform);

		renderableObject->transform.SetPosition(position);
		renderableObject->material.SetColor(color);

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

		m_projectionUniformLocation = m_shaderProgram->GetUniform(m_renderDescriptor.projectionUniform);
		m_viewUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.viewUniform);

		m_camera = camera;
	}

	void RenderSequence::Update(GLfloat deltaTime) {
		glUseProgram(m_shaderProgram->GetObject());

		m_camera->Update();
		const glm::mat4& projectionMatrix	= m_camera->GetProjection();
		const glm::mat4& viewMatrix			= m_camera->GetViewTransform();
		glUniformMatrix4fv(m_projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(m_viewUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		for (const auto& gameObject : m_gameObjects) {
			gameObject->Update(deltaTime);

			RenderableObject* renderableObject = gameObject->GetRenderableObject();

			renderableObject->transform.Update();
			const glm::mat4& modelMatrix = renderableObject->transform.GetModelMatrix();
			glUniformMatrix4fv(renderableObject->transformUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			const glm::vec3& materialColor = renderableObject->material.GetColor();
			glUniform3f(renderableObject->colorUniformLocation, materialColor.r, materialColor.g, materialColor.b);

			glBindVertexArray(renderableObject->vertexArrayObject);
			glDrawElements(renderableObject->drawCall.drawMode, renderableObject->drawCall.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}