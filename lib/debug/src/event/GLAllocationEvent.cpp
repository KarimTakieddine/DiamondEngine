#include "GLAllocationEvent.h"

namespace diamond_engine
{
	GLAllocationEvent::GLAllocationEvent(GLuint* currentTop, GLuint* newTop, GLsizei objectCount) :
		m_currentTop(currentTop), m_newTop(newTop), m_objectCount(objectCount) { }

	const GLuint* GLAllocationEvent::getCurrentTop() const
	{
		return m_currentTop;
	}

	void GLAllocationEvent::setCurrentTop(GLuint* currentTop)
	{
		m_currentTop = currentTop;
	}

	const GLuint* GLAllocationEvent::getNewTop() const
	{
		return m_newTop;
	}

	void GLAllocationEvent::setNewTop(GLuint* newTop)
	{
		m_newTop = newTop;
	}

	GLsizei GLAllocationEvent::getObjectCount() const
	{
		return m_objectCount;
	}

	void GLAllocationEvent::setObjectCount(GLsizei objectCount)
	{
		m_objectCount = objectCount;
	}
}