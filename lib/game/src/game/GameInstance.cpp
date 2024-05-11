#include "GameInstance.h"

namespace diamond_engine
{
	void GameInstance::setRenderObject(RenderObject* renderObject)
	{
		for (const auto& behaviourComponent : m_behaviourComponents)
		{
			behaviourComponent->setRenderObject(renderObject);
		}

		for (const auto& renderComponent : m_renderComponents)
		{
			renderComponent->onRenderObjectAllocated(renderObject);
		}

		m_renderObject = renderObject;
	}

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

		renderComponent->onRenderObjectAllocated(m_renderObject);

		m_renderComponents.push_back(std::move(renderComponent));

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

		behaviourComponent->setRenderObject(m_renderObject);

		m_behaviourComponents.push_back(std::move(behaviourComponent));

		return { };
	}

	const std::vector<std::unique_ptr<IRenderComponent>>& GameInstance::getRenderComponents() const
	{
		return m_renderComponents;
	}

	const std::vector<std::unique_ptr<BehaviourComponent>>& GameInstance::getBehaviourComponents() const
	{
		return m_behaviourComponents;
	}
}