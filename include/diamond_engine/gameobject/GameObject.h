#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "AnimationPlayer.h"
#include "Behaviour.h"
#include "Component.h"
#include "RenderableObject.h"

namespace diamond_engine {
class GameObject {
public:
	void AcquireComponent(std::unique_ptr<Component> component);

	void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram);

	void SetRenderableObject(RenderableObject* renderableObject);

	const RenderableObject* GetRenderableObject() const;

	RenderableObject* GetRenderableObject();
	
	void SetParent(GameObject* parent);
	void OnAddedToScene();
	void Update(GLfloat deltaTime);

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

	void AddBehaviour(std::unique_ptr<Behaviour> behaviour);

	void OnCollisionEnter(const glm::vec2& resolutionAxis, const std::string& name);
	void OnCollisionExit(const std::string& name);

	const std::string& getName() const;

	void setName(const std::string& name);

private:
	std::vector<std::unique_ptr<Component>> m_components{ };
	std::vector<std::unique_ptr<Behaviour>> m_behaviours{ };
	std::string m_name{ "" };
	std::unique_ptr<AnimationPlayer> m_animationPlayer{ nullptr };
	RenderableObject* m_renderableObject{ nullptr };
	GameObject* m_parent{ nullptr };
};
}
