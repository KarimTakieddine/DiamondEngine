#include "GameInstance.h"

namespace diamond_engine
{
	EngineStatus GameInstance::acquireRenderComponent(std::unique_ptr<IRenderComponent> renderComponent)
	{
		if (!renderComponent)
		{
			return { "Cannot acquire null render component", true };
		}

		if (getRenderComponent<IRenderComponent>(renderComponent->getName()))
		{
			return { "Cannot acquire two render components of the same type", true };
		}

		m_renderComponents.push_back(std::move(renderComponent));

		return { };
	}

	EngineStatus GameInstance::removeRenderComponent(const std::string& name)
	{
		auto it = getRenderComponentIt(name);

		if (it == m_renderComponents.cend())
		{
			return { "Render component removal failed. Not found: " + name, true };
		}

		m_renderComponents.erase(it);

		return { };
	}

	EngineStatus GameInstance::acquireBehaviourComponent(std::unique_ptr<BehaviourComponent> behaviourComponent)
	{
		if (!behaviourComponent)
		{
			return { "Cannot acquire null behaviour component", true };
		}

		if (getBehaviourComponent<BehaviourComponent>(behaviourComponent->getName()))
		{
			return { "Cannot acquire two behaviour components of the same type", true };
		}

		m_behaviourComponents.push_back(std::move(behaviourComponent));

		return { };
	}

	EngineStatus GameInstance::removeBehaviourComponent(const std::string& name)
	{
		auto it = getBehaviourComponentIt(name);

		if (it == m_behaviourComponents.cend())
		{
			return { "Behaviour component removal failed. Not found: " + name, true };
		}

		m_behaviourComponents.erase(it);

		return { };
	}

	const std::vector<std::unique_ptr<IRenderComponent>>& GameInstance::getRenderComponents() const
	{
		return m_renderComponents;
	}

	std::vector<std::unique_ptr<IRenderComponent>>& GameInstance::getRenderComponents()
	{
		return m_renderComponents;
	}

	std::vector<std::unique_ptr<BehaviourComponent>>& GameInstance::getBehaviourComponents()
	{
		return m_behaviourComponents;
	}

	const std::vector<std::unique_ptr<BehaviourComponent>>& GameInstance::getBehaviourComponents() const
	{
		return m_behaviourComponents;
	}

	std::unique_ptr<BehaviourComponent> GameInstance::extractBehaviourComponent(const std::string& name)
	{
		auto componentIt = getBehaviourComponentIt(name);

		if (componentIt == m_behaviourComponents.cend())
		{
			return nullptr;
		}

		const auto offset = std::distance(m_behaviourComponents.cbegin(), componentIt);
		auto resultIt = std::next(m_behaviourComponents.begin(), offset);

		std::unique_ptr<BehaviourComponent> result = std::move(*resultIt);

		m_behaviourComponents.erase(resultIt);

		return result;
	}

	void GameInstance::onCollisionEnter2D(const glm::vec2& resolution, const std::string& name)
	{
		for (const auto& behaviour : m_behaviourComponents)
		{
			behaviour->onCollisionEnter2D(resolution, name);
		}
	}

	void GameInstance::onCollisionExit2D(const std::string& name)
	{
		for (const auto& behaviour : m_behaviourComponents)
		{
			behaviour->onCollisionExit2D(name);
		}
	}

	const std::string& GameInstance::getName() const
	{
		return m_name;
	}

	void GameInstance::setName(const std::string& name)
	{
		m_name = name;
	}

	const std::string& GameInstance::getInternalName() const
	{
		return m_internalName;
	}

	void GameInstance::setInternalName(const std::string& name)
	{
		m_internalName = name;
	}

	bool GameInstance::isActive() const
	{
		return m_isActive;
	}

	void GameInstance::setActive(bool active)
	{
		m_isActive = active;
	}
}