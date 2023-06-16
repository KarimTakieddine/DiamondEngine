#include <stdexcept>

#include "GameObject.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace diamond_engine {
	// GameObject::GameObject() : m_components()/*, m_shaderProgram(std::make_unique<ShaderProgram>())*/ { }

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

	void GameObject::ReleaseComponent(std::unique_ptr<Component> component) {
		auto element = std::find_if(m_components.begin(), m_components.end(),
			[&](auto& element) { return element->GetName() == component->GetName(); });

		if (element == m_components.end()) {
			return;
		}

		(*element)->UnsetGameObject();

		m_components.erase(element);
	}

	void GameObject::SetRenderableObject(RenderableObject* renderableObject)
	{
		m_renderableObject = renderableObject;
	}

	const RenderableObject* GameObject::GetRenderableObject() const {
		return m_renderableObject;
	}

	RenderableObject* GameObject::GetRenderableObject() {
		return m_renderableObject;
	}

	void GameObject::OnAddedToScene() {
		/*m_shaderProgram->Link();

		if (!m_shaderProgram->IsLinked()) {
			throw std::runtime_error("Unable to link GameObject ShaderProgram");
		}

		glUseProgram(m_shaderProgram->GetObject());*/
		for (auto& component : m_components) {
			component->BindToContext();
		}
	}

	void GameObject::Update(GLfloat deltaTime) {
		//glUseProgram(m_shaderProgram->GetObject());

		for (auto& component : m_components) {
			component->Update(deltaTime);
		}
	}

	void GameObject::Render() {
		//glUseProgram(m_shaderProgram->GetObject());
		for (auto& component : m_components) {
			component->Render();
		}
	}
}