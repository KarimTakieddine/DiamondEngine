#pragma once

#include <memory>
#include <vector>

#include "Component.h"

namespace diamond_engine {
class GameObject {
public:
	void AcquireComponent(std::unique_ptr<Component> component);

private:
	std::vector<std::unique_ptr<Component>> m_components;
};
}
