#pragma once

#include <memory>

namespace diamond_engine {
class GameObject;
class GameObjectBuilder;

class GameObjectCreator {
public:
	static std::unique_ptr<GameObject> Create(GameObjectBuilder* gameObjectBuilder);
};
}
