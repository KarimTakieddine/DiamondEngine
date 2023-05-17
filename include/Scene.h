#pragma once

#include <vector>

#include "Camera.h"
#include "GameObject.h"

namespace diamond_engine {
class Scene {
public:
	explicit Scene();

	void AddGameObject(std::unique_ptr<GameObject> gameObject);

	void Update(GLfloat deltaTime);
	void Render();
	void OnWindowResize(int width, int height);

private:
	std::unique_ptr<Camera> m_camera{ nullptr };
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};
}
