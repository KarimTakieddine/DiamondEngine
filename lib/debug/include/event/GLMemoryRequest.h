#pragma once

#include <GL/glew.h>

#include "DebugEvent.h"

namespace diamond_engine
{
	class GLMemoryRequest : public DebugEvent
	{
	public:
		~GLMemoryRequest() final override = default;

		GLMemoryRequest(GLuint* location, GLuint value);

		const GLuint* getLocation() const;
		void setLocation(GLuint* location);

		GLuint getValue() const;
		void setValue(GLuint value);

	private:
		GLuint* m_location{ nullptr };
		GLuint m_value{ 0 };
	};
}
