#pragma once

#include <memory>

namespace diamond_engine {
class GameObject;
class GameObjectBuilder;

class GameObjectDesigner {
public:
	static void Design(
		GameObject* gameObject,
		GameObjectBuilder* gameObjectBuilder);
};
}
