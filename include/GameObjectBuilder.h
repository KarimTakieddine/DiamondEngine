#pragma once

#include "GameObject.h"
#include "GLAllocator.h"

namespace diamond_engine {
class GameObjectConfig;

class GameObjectBuilder {
public:
	void SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator);

	std::unique_ptr<GameObject> Build(const GameObjectConfig* gameObjectConfig);

private:
	std::shared_ptr<GLAllocator> m_bufferAllocator{ nullptr };
};
}
