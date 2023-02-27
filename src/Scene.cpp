#include <GL/glew.h>

#include "Scene.h"

namespace diamond_engine {
	void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
		gameObject->OnAddedToScene();

		m_gameObjects.push_back(std::move(gameObject));
	}

	void Scene::Update() {
		for (auto& gameObject : m_gameObjects) {
			gameObject->Update();
		}
	}

	void Scene::Render() {
		// TODO: Make  this a member?
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& gameObject : m_gameObjects) {
			gameObject->Render();
		}
	}
}