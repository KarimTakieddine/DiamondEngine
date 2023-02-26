#include <algorithm>
#include <stdexcept>

#include "Component.h"
#include "GameObject.h"

namespace diamond_engine {
	GameObject::GameObject() : m_components(), m_shaderProgram(std::make_unique<ShaderProgram>()) { }

	void GameObject::AcquireComponent(std::unique_ptr<Component> component) {
		if (std::find_if(
			m_components.begin(),
			m_components.end(), [&](auto& element) {
				return element->GetName() == component->GetName();
			}) != m_components.end()) {
			throw std::runtime_error("Cannot add two identical components of: " + std::string(component->GetName()) + " type");
		}

		component->AttachToProgram(m_shaderProgram.get());

		m_components.push_back(std::move(component));
	}

	void GameObject::ReleaseComponent(std::unique_ptr<Component> component) {
		auto element = std::find_if(m_components.begin(), m_components.end(),
			[&](auto& element) { return element->GetName() == component->GetName(); });

		if (element == m_components.end()) {
			return;
		}

		(*element)->DetachFromProgram(m_shaderProgram.get());

		m_components.erase(element);
	}
}