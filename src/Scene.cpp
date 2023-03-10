#include <stdexcept>

#include <GL/glew.h>

#include "GameObjectDesigner.h"
#include "Scene.h"

namespace diamond_engine {
	Scene::Scene() : m_camera(std::make_unique<Camera>()), m_gameObjects() {
		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(5.0f, 1.333f, 0.1f, 100.0f);
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
		gameObject->OnAddedToScene();

		m_camera->SetGameObject(gameObject.get());
		m_camera->BindToContext();

		m_gameObjects.push_back(std::move(gameObject));
	}

	void Scene::Update(GLfloat deltaTime) {
		for (auto& gameObject : m_gameObjects) {
			m_camera->Update(deltaTime);

			gameObject->Update(deltaTime);
		}
	}

	void Scene::Render() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// TODO: Make  this a member?
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& gameObject : m_gameObjects) {
			gameObject->Render();
		}
	}
}