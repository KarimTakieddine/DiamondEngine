#include "Collider2DComponentConfig.h"

namespace diamond_engine
{
	const char* Collider2DComponentConfig::getName() const
	{
		return "Collider2D";
	}

	const glm::vec2& Collider2DComponentConfig::getSize() const
	{
		return m_size;
	}

	void Collider2DComponentConfig::setSize(const glm::vec2& size)
	{
		m_size = size;
	}

	const glm::vec2& Collider2DComponentConfig::getOffset() const
	{
		return m_offset;
	}

	void Collider2DComponentConfig::setOffset(const glm::vec2& offset)
	{
		m_offset = offset;
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

	ColliderIgnoreFlags Collider2DComponentConfig::getIgnoreFlags() const
	{
		return m_ignoreFlags;
	}

	void Collider2DComponentConfig::setIgnoreFlags(ColliderIgnoreFlags flags)
	{
		m_ignoreFlags = flags;
	}
}