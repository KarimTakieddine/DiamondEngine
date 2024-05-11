#pragma once

#include <functional>
#include <string>

#include <GL/glew.h>

namespace diamond_engine {
class GLAllocator {
public:
	using GLFunction = std::function<void(GLsizei, GLuint*)>;

	GLAllocator(GLFunction allocateFunction, GLFunction freeFunction);
	
	void Reserve(GLsizei capacity);

	void Allocate(GLsizei objectCount);

	void Free(GLsizei objectCount);

	GLuint Get();

	GLsizeiptr GetAllocatedObjectCount() const;

	GLsizeiptr GetLiveObjectCount() const;

	std::string ToString() const;

	~GLAllocator();

private:
	GLFunction m_allocateFunction	{ nullptr };
	GLFunction m_freeFunction		{ nullptr };
	GLuint* m_objectTopBounds		{ nullptr };
	GLuint* m_memoryTopBounds		{ nullptr };
	GLuint* m_current				{ nullptr };
	GLuint* m_memory				{ nullptr };
};
}
