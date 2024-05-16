#include "TransformComponentConfig.h"

namespace diamond_engine
{
	const char* TransformComponentConfig::getName() const
	{
		return "Transform";
	}

	const glm::vec3& TransformComponentConfig::getPosition() const
	{
		return m_position;
	}

	void TransformComponentConfig::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}

	const glm::vec3& TransformComponentConfig::getLocalScale() const
	{
		return m_localScale;
	}

	void TransformComponentConfig::setLocalScale(const glm::vec3& localScale)
	{
		m_localScale = localScale;
	}
}