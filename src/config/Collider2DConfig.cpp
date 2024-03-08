#include "Collider2DConfig.h"

namespace diamond_engine
{
	GLfloat Collider2DConfig::getWidth() const
	{
		return m_width;
	}

	void Collider2DConfig::setWidth(GLfloat width)
	{
		m_width = width;
	}

	GLfloat Collider2DConfig::getHeight() const
	{
		return m_height;
	}

	void Collider2DConfig::setHeight(GLfloat height)
	{
		m_height = height;
	}

	ColliderType Collider2DConfig::getColliderType() const
	{
		return m_colliderType;
	}

	void Collider2DConfig::setColliderType(ColliderType colliderType)
	{
		m_colliderType = colliderType;
	}
}