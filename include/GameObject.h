#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "Component.h"
#include "ShaderProgram.h"

namespace diamond_engine {
class GameObject {
public:
	GameObject();

	void AcquireComponent(std::unique_ptr<Component> component);
	void ReleaseComponent(std::unique_ptr<Component> component);
	
	void OnAddedToScene();
	void Update(GLfloat deltaTime);
	void Render();

	template<typename T>
	T* GetComponent(const std::string& name) const {
		auto result = std::find_if(
			m_components.begin(),
			m_components.end(),
			[&](const std::unique_ptr<Component>& component) { return std::string(component->GetName()) == name; });

		if (result == m_components.end()) {
			return nullptr;
		}

		return dynamic_cast<T*>(result->get());
	}

	ShaderProgram* GetShaderProgram() { return m_shaderProgram.get(); }

private:
	std::vector<std::unique_ptr<Component>> m_components;
	std::unique_ptr<ShaderProgram> m_shaderProgram{ nullptr };
};
}
