#pragma once

namespace diamond_engine {
class BehaviourConfig {
public:
	virtual ~BehaviourConfig() = default;

	virtual const char* GetName() const = 0;
};
}
