#include "TransformConfig.h"

namespace diamond_engine
{
	const glm::vec3& TransformConfig::getPosition() const
	{
		return m_position;
	}

	glm::vec3& TransformConfig::getPosition()
	{
		return m_position;
	}

	void TransformConfig::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}
}