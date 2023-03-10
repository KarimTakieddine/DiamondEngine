#include "GameObjectBuilder.h"

// TODO: Possibly delete this source file later?

namespace diamond_engine {
	void GameObjectBuilder::SetGameObject(GameObject* gameObject) {
		m_gameObject = gameObject;
	}
	
	void GameObjectBuilder::SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) {
		m_sharedShaderStore = sharedShaderStore;
	}
}