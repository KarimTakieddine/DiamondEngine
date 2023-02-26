#include "GameObjectDesigner.h"

namespace diamond_engine {
	/* static */ void GameObjectDesigner::Design(GameObject* gameObject, GameObjectBuilder* gameObjectBuilder) {
		gameObjectBuilder->SetGameObject(gameObject);
		gameObjectBuilder->AddComponents();
	}
}