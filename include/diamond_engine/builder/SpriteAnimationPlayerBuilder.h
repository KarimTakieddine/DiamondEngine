#pragma once

#include <memory>

#include "Component.h"
#include "GLAllocator.h"

namespace diamond_engine {
	class ComponentConfig;

	class SpriteAnimationPlayerBuilder
	{
	public:
		static std::unique_ptr<Component> Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const ComponentConfig* componentConfig);
	};
}
