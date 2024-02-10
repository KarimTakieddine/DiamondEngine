#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"

namespace diamond_engine {
	void GameObject::AcquireComponent(std::unique_ptr<Component> component) {
		if (std::find_if(
			m_components.begin(),
			m_components.end(), [&](auto& element) {
				return element->GetName() == component->GetName();
			}) != m_components.end()) {
			throw std::runtime_error("Cannot add two identical components of: " + std::string(component->GetName()) + " type");
		}

		component->SetGameObject(this);

		m_components.push_back(std::move(component));
	}

	void GameObject::BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) {
		for (const auto& component : m_components) {
			component->BindToShaderProgram(shaderProgram);
		}
	}

	void GameObject::SetRenderableObject(RenderableObject* renderableObject) {
		m_renderableObject = renderableObject;
	}

	const RenderableObject* GameObject::GetRenderableObject() const {
		return m_renderableObject;
	}

	RenderableObject* GameObject::GetRenderableObject() {
		return m_renderableObject;
	}

	void GameObject::SetParent(GameObject* parent)
	{
		m_parent = parent;
	}

	void GameObject::OnAddedToScene() {
		for (auto& component : m_components) {
			component->OnAddedToScene();
		}
	}

	void GameObject::Update(GLfloat deltaTime) {
		if (m_parent && m_renderableObject)
		{
			RenderableObject* const parentRederableObject = m_parent->GetRenderableObject();;

			if (parentRederableObject)
			{
				m_renderableObject->transform = parentRederableObject->transform;
			}
		}

		for (auto& behaviour : m_behaviours) {
			behaviour->Update(deltaTime);
		}
	}

	void GameObject::AddBehaviour(std::unique_ptr<Behaviour> behaviour) {
		behaviour->SetGameObject(this);

		m_behaviours.push_back(std::move(behaviour));
	}
}