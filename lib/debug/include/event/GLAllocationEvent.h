#pragma once

#include <GL/glew.h>

#include "DebugEvent.h"

namespace diamond_engine
{
	class GLAllocationEvent : public DebugEvent
	{
	public:
		~GLAllocationEvent() final override = default;

		GLAllocationEvent(GLuint* currentTop, GLuint* newTop, GLsizei objectCount);

		const GLuint* getCurrentTop() const;
		void setCurrentTop(GLuint* currentTop);

		const GLuint* getNewTop() const;
		void setNewTop(GLuint* newTop);

		GLsizei getObjectCount() const;
		void setObjectCount(GLsizei objectCount);

	private:
		GLuint* m_currentTop	{ nullptr };
		GLuint* m_newTop		{ nullptr };
		GLsizei m_objectCount	{ 0 };
	};
}
