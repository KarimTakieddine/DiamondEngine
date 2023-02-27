#pragma once

#include <memory>

#include "GameObjectBuilder.h"
#include "GLAllocator.h"
#include "SharedShaderStore.h"

namespace diamond_engine {
class SpriteBuilder : public GameObjectBuilder {
public:
	SpriteBuilder();

	void SetCapacity(GLsizei capacity) override;

	void AddComponents() override;

	void SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

private:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore;
	std::unique_ptr<GLAllocator> m_vertexArrayAllocator;
	std::unique_ptr<GLAllocator> m_bufferAllocator;
};
}
