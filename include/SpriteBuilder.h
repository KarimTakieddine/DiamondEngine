#pragma once

#include "GameObjectBuilder.h"
#include "GLAllocator.h"

namespace diamond_engine {
class SpriteBuilder : public GameObjectBuilder {
public:
	SpriteBuilder();

	void SetCapacity(GLsizei capacity);

	void AddComponents() override;

private:
	std::unique_ptr<GLAllocator> m_vertexArrayAllocator;
	std::unique_ptr<GLAllocator> m_bufferAllocator;
};
}
