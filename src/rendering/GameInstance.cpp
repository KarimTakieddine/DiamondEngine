#include "GameInstance.h"

namespace diamond_engine
{
	void GameInstance::setRenderObject(RenderObject* renderObject)
	{
		m_renderObject = renderObject;
	}

	EngineStatus GameInstance::acquireRenderComponent(std::unique_ptr<IRenderComponent> renderComponent)
	{
		if (getRenderComponent<IRenderComponent>(renderComponent->getComponentType()))
		{
			return { "Cannot acquire two components of the same type", true };
		}

		m_renderComponents.push_back(std::move(renderComponent));

		return { };
	}

	const std::vector<std::unique_ptr<IRenderComponent>>& GameInstance::getRenderComponents() const
	{
		return m_renderComponents;
	}

	RenderObject* GameInstance::getRenderObject() const
	{
		return m_renderObject;
	}
}