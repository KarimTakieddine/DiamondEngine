#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"

namespace diamond_engine {
class Scene {
public:
	void AddGameObject(std::unique_ptr<GameObject> gameObject);

	void Update();
	void Render();

private:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};
}
