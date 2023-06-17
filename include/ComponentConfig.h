#pragma once

#include "ComponentType.h"

namespace diamond_engine {
class ComponentConfig {
public:
	virtual ~ComponentConfig() = default;

	virtual ComponentType GetType() const = 0;
};
}
