#pragma once

#include <glm/vec2.hpp>

#include "Behaviour.h"

namespace diamond_engine
{
	class ScrollingBackgroundBehaviour : public Behaviour
	{
	public:
		~ScrollingBackgroundBehaviour() override = default;

		void Update(GLfloat deltaTime) override;

		void setScrollingSpeed(const glm::vec2& speed);

	private:
		glm::vec2 m_scrollingSpeed{ 0.0f, 0.0f };
	};
}
