#pragma once

#include <memory>

#include "allocator/GLAllocator.h"

namespace diamond_engine {
class ComponentConfig;

class MeshRendererBuilder {
public:
	static std::unique_ptr<Component> Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const ComponentConfig* componentConfig);
};
}
