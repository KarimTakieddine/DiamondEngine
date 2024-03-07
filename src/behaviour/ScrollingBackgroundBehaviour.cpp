#include "GameObject.h"
#include "ScrollingBackgroundBehaviour.h"

namespace diamond_engine
{
	void ScrollingBackgroundBehaviour::Update(GLfloat deltaTime)
	{
		Material& material = m_gameObject->GetRenderableObject()->material;

		const glm::vec2& materialTextureOffset = material.GetTextureOffset();
		material.SetTextureOffset(materialTextureOffset + (m_scrollingSpeed * deltaTime));
	}

	void ScrollingBackgroundBehaviour::setScrollingSpeed(const glm::vec2& speed)
	{
		m_scrollingSpeed = speed;
	}
}