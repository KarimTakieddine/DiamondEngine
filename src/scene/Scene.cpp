#include <stdexcept>

#include <GL/glew.h>

#include "scene/Scene.h"

namespace diamond_engine {
	Scene::Scene(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) :
		m_sharedShaderStore(sharedShaderStore),
		m_vertexArrayAllocator(std::make_shared<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays)),
		m_bufferAllocator(std::make_shared<GLAllocator>(glGenBuffers, glDeleteBuffers)),
		m_camera(std::make_shared<Camera>()),
		m_renderableObjectAllocator(std::make_shared<ObjectAllocator>()),
		m_spriteRenderSequence(std::make_unique<RenderSequence>()) {
		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(60.0f, 1.333f, 0.3f, 1000.0f);

		const std::shared_ptr<ShaderProgram>& spriteShaderProgram = m_sharedShaderStore->FindProgram("sprite");

		if (!spriteShaderProgram) {
			throw std::runtime_error("Failed to locate \"sprite\" shader in shared shader store instance");
		}

		m_spriteRenderSequence->SetRenderDescriptor(
			{
				"projection",
				"cameraLocalToWorld",
				"cameraLocalRotation",
				"cameraView",
				"objectLocalToWorld",
				"objectLocalRotation",
				"objectLocalScale",
				"materialColor"
			}
		);
		m_spriteRenderSequence->SetShaderProgram(spriteShaderProgram);
		m_spriteRenderSequence->SetCamera(m_camera);
		m_spriteRenderSequence->SetObjectAllocator(m_renderableObjectAllocator);
		m_spriteRenderSequence->SetVertexArrayAllocator(m_vertexArrayAllocator);
	}

	void Scene::SetMaxObjects(GLint maxObjects) {
		m_vertexArrayAllocator->Reserve(static_cast<GLsizei>(maxObjects));
		m_vertexArrayAllocator->Allocate(static_cast<GLsizei>(maxObjects));

		m_bufferAllocator->Reserve(static_cast<GLsizei>(maxObjects << 1));
		m_bufferAllocator->Allocate(static_cast<GLsizei>(maxObjects << 1));

		m_renderableObjectAllocator->Allocate(static_cast<size_t>(maxObjects));
		m_renderableObjectAllocator->Expand(static_cast<size_t>(maxObjects));
	}

	const std::shared_ptr<GLAllocator>& Scene::GetBufferAllocator() const {
		return m_bufferAllocator;
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject, const glm::vec3& position, const glm::vec3& color, GameObjectType type) {
		switch (type) {
		case GameObjectType::SPRITE: {
			m_spriteRenderSequence->AddGameObject(std::move(gameObject), position, color);
			break;
		}
		default:
			break;
		}
	}

	void Scene::Update(GLfloat deltaTime) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_spriteRenderSequence->Update(deltaTime);
	}

	void Scene::OnWindowResize(int width, int height) {
		m_camera->SetAspectRatio(static_cast<GLfloat>(width) / height);
	}
}