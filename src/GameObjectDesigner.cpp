#include "GameObjectBuilder.h"
#include "GameObjectDesigner.h"

namespace diamond_engine {
	/* static */ void GameObjectDesigner::Design(GameObject* gameObject, GameObjectBuilder* gameObjectBuilder) {
		gameObjectBuilder->SetTarget(gameObject);
		gameObjectBuilder->AddComponents();
	}
}