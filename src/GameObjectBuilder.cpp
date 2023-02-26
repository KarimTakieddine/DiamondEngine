#include "GameObject.h"
#include "GameObjectBuilder.h"

namespace diamond_engine {
	void GameObjectBuilder::Reset() {
		m_gameObject.reset(new GameObject());
	}

	std::unique_ptr<GameObject> GameObjectBuilder::Finalize() {
		return std::move(m_gameObject);
	}
}