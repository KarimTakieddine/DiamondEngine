#include "EngineVariant.h"

namespace diamond_engine
{
	EngineVariant::EngineVariant(GLint value) : m_value(value) {}

	EngineVariant::EngineVariant(GLuint value) : m_value(value) {}

	EngineVariant::EngineVariant(GLfloat value) : m_value(value) {}

	EngineVariant::EngineVariant(GLdouble value) : m_value(value) {}

	EngineVariant::EngineVariant(std::string value) : m_value(value) {}

	GLint EngineVariant::asInt() const
	{
		return std::get<GLint>(m_value);
	}

	GLuint EngineVariant::asUInt() const
	{
		return std::get<GLuint>(m_value);
	}

	GLfloat EngineVariant::asFloat() const
	{
		return std::get<GLfloat>(m_value);
	}

	GLdouble EngineVariant::asDouble() const
	{
		return std::get<GLdouble>(m_value);
	}

	const std::string& EngineVariant::asString() const
	{
		return std::get<std::string>(m_value);
	}

	EngineVariant& EngineVariant::operator=(GLint value)
	{
		m_value = value;

		return *this;
	}

	EngineVariant& EngineVariant::operator=(GLuint value)
	{
		m_value = value;
		
		return *this;
	}

	EngineVariant& EngineVariant::operator=(GLfloat value)
	{
		m_value = value;

		return *this;
	}

	EngineVariant& EngineVariant::operator=(GLdouble value)
	{
		m_value = value;

		return *this;
	}

	EngineVariant& EngineVariant::operator=(const std::string& value)
	{
		m_value = value;

		return *this;
	}
}