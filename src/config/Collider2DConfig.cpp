#include "Collider2DConfig.h"

namespace diamond_engine
{
	GLuint Collider2DConfig::getWidth() const
	{
		return m_width;
	}

	void Collider2DConfig::setWidth(GLuint width)
	{
		m_width = width;
	}

	GLuint Collider2DConfig::getHeight() const
	{
		return m_height;
	}

	void Collider2DConfig::setHeight(GLuint height)
	{
		m_height = height;
	}
}