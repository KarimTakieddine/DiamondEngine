#pragma once

#include <GL/glew.h>

#include <string>
#include <variant>

namespace diamond_engine
{
	using Variant = std::variant<GLint, GLuint, GLfloat, GLdouble, std::string>;

	class EngineVariant
	{
	public:
		EngineVariant() = delete;

		explicit EngineVariant(GLint value);
		explicit EngineVariant(GLuint value);
		explicit EngineVariant(GLfloat value);
		explicit EngineVariant(GLdouble value);
		explicit EngineVariant(std::string value);

		GLint asInt() const;
		GLuint asUInt() const;
		GLfloat asFloat() const;
		GLdouble asDouble() const;
		const std::string& asString() const;

		EngineVariant& operator=(GLint value);
		EngineVariant& operator=(GLuint value);
		EngineVariant& operator=(GLfloat value);
		EngineVariant& operator=(GLdouble value);
		EngineVariant& operator=(const std::string& value);

	private:
		Variant m_value;
	};
}
