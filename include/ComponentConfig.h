#pragma once

namespace diamond_engine {
class ComponentConfig {
public:
	virtual ~ComponentConfig() = default;

	virtual const char* GetName() const = 0;
};
}
