#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "IRenderComponent.h"

namespace diamond_engine
{
	class RenderObject;
	class GameInstance
	{
	public:
		void setRenderObject(RenderObject* renderObject);
		EngineStatus acquireRenderComponent(std::unique_ptr<IRenderComponent> renderComponent);

		template<typename T>
		T* getRenderComponent(RenderComponentType componentType) const
		{
			auto componentIt = std::find_if(
				m_renderComponents.begin(),
				m_renderComponents.end(),
				[componentType](const std::unique_ptr<IRenderComponent>& renderComponent)
				{
					return renderComponent->getComponentType() == componentType;
				});

			return componentIt == m_renderComponents.cend() ? nullptr : dynamic_cast<T*>(componentIt->get());
		}

	private:
		std::vector<std::unique_ptr<IRenderComponent>> m_renderComponents;
		RenderObject* m_renderObject{ nullptr };
	};
}
