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

		renderableObject->vertexArrayObject						= m_vertexArrayAllocator->Get();
		renderableObject->objectLocalToWorldUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.objectLocalToWorldUniform);
		renderableObject->objectLocalRotationUniformLocation	= m_shaderProgram->GetUniform(m_renderDescriptor.objectLocalRotationUniform);
		renderableObject->objectLocalScaleUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.objectLocalScaleUniform);
		renderableObject->colorUniformLocation					= m_shaderProgram->GetUniform(m_renderDescriptor.colorUniform);

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

		m_projectionUniformLocation				= m_shaderProgram->GetUniform(m_renderDescriptor.projectionUniform);
		m_cameraLocalToWorldUniformLocation		= m_shaderProgram->GetUniform(m_renderDescriptor.cameraLocalToWorldUniform);
		m_cameraLocalRotationUniformLocation	= m_shaderProgram->GetUniform(m_renderDescriptor.cameraLocalRotationUniform);
		m_cameraViewUniformLocation				= m_shaderProgram->GetUniform(m_renderDescriptor.cameraViewUniform);

		m_camera = camera;
	}

	void RenderSequence::Update(GLfloat deltaTime) {
		glUseProgram(m_shaderProgram->GetObject());

		const glm::mat4& projectionMatrix			= m_camera->GetProjection();
		const glm::mat4& cameraLocalToWorldMatrix	= m_camera->GetTransform().GetLocalToWorldMatrix();
		const glm::mat4& cameraLocalRotationMatrix	= m_camera->GetTransform().GetLocalRotationMatrix();
		const glm::mat4& cameraViewMatrix			= m_camera->GetView();

		glUniformMatrix4fv(m_projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(m_cameraLocalToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraLocalToWorldMatrix));
		glUniformMatrix4fv(m_cameraLocalRotationUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraLocalRotationMatrix));
		glUniformMatrix4fv(m_cameraViewUniformLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

		for (const auto& gameObject : m_gameObjects) {
			gameObject->Update(deltaTime);

			RenderableObject* renderableObject = gameObject->GetRenderableObject();

			const glm::mat4& objectLocalToWorldMatrix = renderableObject->transform.GetLocalToWorldMatrix();
			glUniformMatrix4fv(renderableObject->objectLocalToWorldUniformLocation, 1, GL_FALSE, glm::value_ptr(objectLocalToWorldMatrix));

			const glm::mat4& objectLocalRotationMatrix = renderableObject->transform.GetLocalRotationMatrix();
			glUniformMatrix4fv(renderableObject->objectLocalRotationUniformLocation, 1, GL_FALSE, glm::value_ptr(objectLocalRotationMatrix));

			const glm::mat4& objectLocalScaleMatrix = renderableObject->transform.GetLocalScaleMatrix();
			glUniformMatrix4fv(renderableObject->objectLocalScaleUniformLocation, 1, GL_FALSE, glm::value_ptr(objectLocalScaleMatrix));

			const glm::vec3& materialColor = renderableObject->material.GetColor();
			glUniform3f(renderableObject->colorUniformLocation, materialColor.r, materialColor.g, materialColor.b);

			glBindVertexArray(renderableObject->vertexArrayObject);
			glDrawElements(renderableObject->drawCall.drawMode, renderableObject->drawCall.elementCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}