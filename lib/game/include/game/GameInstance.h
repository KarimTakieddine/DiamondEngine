#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "BehaviourComponent.h"
#include "IRenderComponent.h"

namespace diamond_engine
{
	class GameInstance
	{
	public:
		void setRenderObject(RenderObject* renderObject);
		EngineStatus acquireRenderComponent(std::unique_ptr<IRenderComponent> renderComponent);
		EngineStatus acquireBehaviourComponent(std::unique_ptr<BehaviourComponent> behaviourComponent);

		template<typename T>
		T* getRenderComponent(const std::string& name) const
		{
			auto componentIt = std::find_if(
				m_renderComponents.begin(),
				m_renderComponents.end(),
				[name](const std::unique_ptr<IRenderComponent>& renderComponent)
				{
					return std::string(renderComponent->getName()) == name;
				});

			return componentIt == m_renderComponents.cend() ? nullptr : dynamic_cast<T*>(componentIt->get());
		}

		template<typename T>
		T* getBehaviourComponent(const std::string& name) const
		{
			auto componentIt = std::find_if(
				m_behaviourComponents.begin(),
				m_behaviourComponents.end(),
				[name](const std::unique_ptr<BehaviourComponent>& behaviourComponent)
				{
					return std::string(behaviourComponent->getName()) == name;
				});

			return componentIt == m_behaviourComponents.cend() ? nullptr : dynamic_cast<T*>(componentIt->get());
		}

		const std::vector<std::unique_ptr<IRenderComponent>>& getRenderComponents() const;
		const std::vector<std::unique_ptr<BehaviourComponent>>& getBehaviourComponents() const;

	private:
		std::vector<std::unique_ptr<IRenderComponent>> m_renderComponents;
		std::vector<std::unique_ptr<BehaviourComponent>> m_behaviourComponents;
		RenderObject* m_renderObject{ nullptr };
	};
}
