#include "GLMemoryRequest.h"

namespace diamond_engine
{
	GLMemoryRequest::GLMemoryRequest(GLuint* location, GLuint value) : m_location(location), m_value(value) { }

	const GLuint* GLMemoryRequest::getLocation() const
	{
		return m_location;
	}

	void GLMemoryRequest::setLocation(GLuint* location)
	{
		m_location = location;
	}

	void GLMemoryRequest::setValue(GLuint value)
	{
		m_value = value;
	}

	GLuint GLMemoryRequest::getValue() const
	{
		return m_value;
	}
}