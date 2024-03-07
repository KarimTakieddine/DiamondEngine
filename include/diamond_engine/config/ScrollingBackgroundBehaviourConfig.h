#pragma once

#include <glm/vec2.hpp>

#include "BehaviourConfig.h"

namespace diamond_engine {
	class ScrollingBackgroundBehaviourConfig : public BehaviourConfig {
	public:
		~ScrollingBackgroundBehaviourConfig() override = default;

		const char* GetName() const override {
			return "ScrollingBackgroundBehaviour";
		}

		void setScrollingSpeed(const glm::vec2& speed)
		{
			m_scrollingSpeed = speed;
		}

		const glm::vec2& getScrollingSpeed() const {
			return m_scrollingSpeed;
		}

	private:
		glm::vec2 m_scrollingSpeed{ 0.0f, 0.0f };
	};
}
