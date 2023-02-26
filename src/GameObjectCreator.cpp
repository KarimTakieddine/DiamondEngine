#include "GameObject.h"
#include "GameObjectBuilder.h"
#include "GameObjectCreator.h"

namespace diamond_engine {
	std::unique_ptr<GameObject> GameObjectCreator::Create(GameObjectBuilder* gameObjectBuilder) {
		gameObjectBuilder->Reset();
		gameObjectBuilder->AddComponents();
		return gameObjectBuilder->Finalize();
	}
}