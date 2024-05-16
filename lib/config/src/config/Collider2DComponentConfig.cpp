#include "Collider2DComponentConfig.h"

namespace diamond_engine
{
	const char* Collider2DComponentConfig::getName() const
	{
		return "Collider2D";
	}

	const Size& Collider2DComponentConfig::getSize() const
	{
		return m_size;
	}

	void Collider2DComponentConfig::setSize(const Size& size)
	{
		m_size = size;
	}

	ColliderType Collider2DComponentConfig::getType() const
	{
		return m_type;
	}

	void Collider2DComponentConfig::setType(ColliderType type)
	{
		m_type = type;
	}

	GLsizei Collider2DComponentConfig::getTargetIndex() const
	{
		return m_targetIndex;
	}

	void Collider2DComponentConfig::setTargetIndex(GLsizei index)
	{
		m_targetIndex = index;
	}
}