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

		std::vector<std::unique_ptr<BehaviourComponent>>::const_iterator getBehaviourComponentIt(const std::string& name) const
		{
			return std::find_if(
				m_behaviourComponents.begin(),
				m_behaviourComponents.end(),
				[name](const std::unique_ptr<BehaviourComponent>& behaviourComponent)
				{
					return std::string(behaviourComponent->getName()) == name;
				});
		}

		template<typename T>
		T* getBehaviourComponent(const std::string& name) const
		{
			auto componentIt = getBehaviourComponentIt(name);

			if (componentIt == m_behaviourComponents.cend())
			{
				return nullptr;
			}

			return dynamic_cast<T*>(componentIt->get());
		}

		std::vector<std::unique_ptr<IRenderComponent>>& getRenderComponents();
		const std::vector<std::unique_ptr<IRenderComponent>>& getRenderComponents() const;
		std::vector<std::unique_ptr<BehaviourComponent>>& getBehaviourComponents();
		const std::vector<std::unique_ptr<BehaviourComponent>>& getBehaviourComponents() const;
		std::unique_ptr<BehaviourComponent> extractBehaviourComponent(const std::string& name);
		void onCollisionEnter2D(const glm::vec2& resolution, const std::string& name);
		void onCollisionExit2D(const std::string& name);

		const std::string& getName() const;
		void setName(const std::string& name);
		const std::string& getInternalName() const;
		void setInternalName(const std::string& name);

	private:
		std::vector<std::unique_ptr<IRenderComponent>> m_renderComponents;
		std::vector<std::unique_ptr<BehaviourComponent>> m_behaviourComponents;
		std::string m_name{ "Unnamed" };
		std::string m_internalName{ "Unnamed" };
	};
}
