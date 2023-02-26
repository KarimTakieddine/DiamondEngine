#include "GameObject.h"

namespace diamond_engine {
	void GameObject::AcquireComponent(std::unique_ptr<Component> component) {
		m_components.push_back(std::move(component));
	}
}