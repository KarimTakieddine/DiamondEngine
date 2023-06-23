#pragma once

#include <memory>

#include "Component.h"
#include "GLAllocator.h"

namespace diamond_engine {
class ComponentConfig;

class ComponentBuilder {
public:
	virtual ~ComponentBuilder() = default;

	void SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator) { m_bufferAllocator = bufferAllocator; }

	virtual std::unique_ptr<Component> Build(const ComponentConfig* componentConfig) = 0;

protected:
	std::shared_ptr<GLAllocator> m_bufferAllocator;
};
}
