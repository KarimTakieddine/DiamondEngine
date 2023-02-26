#pragma once

#include "GameObjectBuilder.h"

namespace diamond_engine {
class GameObjectDesigner {
public:
	static void Design(
		GameObject* gameObject,
		GameObjectBuilder* gameObjectBuilder);
};
}
