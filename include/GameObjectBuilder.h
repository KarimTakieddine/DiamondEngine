#pragma once

#include <GL/glew.h>

#include "GameObject.h"
#include "SharedShaderStore.h"

namespace diamond_engine {
class GameObjectBuilder {
public:
	void SetGameObject(GameObject* gameObject);

	void SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

	virtual void AddComponents() = 0;

	virtual ~GameObjectBuilder() = default;

protected:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore;
	GameObject* m_gameObject{ nullptr };
};
}
