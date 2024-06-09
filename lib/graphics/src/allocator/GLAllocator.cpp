#include <stdexcept>
#include <sstream>

#include "GLAllocator.h"
#include "Debugger.h"
#include "EngineMacros.h"
#include "GLAllocationEvent.h"
#include "GLMemoryRequest.h"

namespace diamond_engine {
	GLAllocator::GLAllocator(GLFunction allocateFunction, GLFunction freeFunction) :
		m_allocateFunction	(allocateFunction),
		m_freeFunction		(freeFunction) { }

	void GLAllocator::Reserve(GLsizei capacity) {
		const auto allocatedObjectCount = GetAllocatedObjectCount();

		if (allocatedObjectCount != 0) {
			throw std::runtime_error(
				"Cannot free GLAllocator memory without freeing: "
				+ std::to_string(allocatedObjectCount) + " GL objects first");
		}

		delete m_memory;

		m_memory			= new GLuint[capacity]();
		m_memoryTopBounds	= m_memory + capacity;
		m_current			= m_memory;
		m_objectTopBounds	= m_current;
	}

	void GLAllocator::Allocate(GLsizei objectCount) {
		if (!m_allocateFunction) {
			throw std::runtime_error("No allocation function assigned to GLAllocator");
		}

		GLuint* const updatedObjectTopBounds = m_objectTopBounds + objectCount;

		if (updatedObjectTopBounds > m_memoryTopBounds) {
			throw std::runtime_error(
				"Attempt to allocate too many GL objects. Can only allocate: "
				+ std::to_string(m_memoryTopBounds - m_objectTopBounds) + " remaining objects");
		}

		m_allocateFunction(objectCount, m_objectTopBounds);

		DEBUG_EXEC(Debugger::getInstance()->debugEvent(
			DebugEvent::Type::GL_OBJECT_ALLOCATION,
			std::make_unique<GLAllocationEvent>(m_objectTopBounds, updatedObjectTopBounds, objectCount )));

		m_objectTopBounds = updatedObjectTopBounds;
	}

	void GLAllocator::Free(GLsizei objectCount) {
		if (objectCount == 0)
		{
			return;
		}

		if (!m_freeFunction) {
			throw std::runtime_error("No freeing function assigned to GLAllocator");
		}

		GLuint* const updatedObjectTopBounds = m_objectTopBounds - objectCount;

		if (updatedObjectTopBounds < m_memory) {
			throw std::runtime_error(
				"Attempt to free too many GL objects. Can only free: "
				+ std::to_string(GetAllocatedObjectCount()) + " remaining objects");
		}

		m_freeFunction(objectCount, updatedObjectTopBounds);

		if (m_current > updatedObjectTopBounds) {
			m_current = updatedObjectTopBounds;
		}

		m_objectTopBounds = updatedObjectTopBounds;
	}

	GLuint GLAllocator::Get() {
		GLuint* const current			= m_current;
		GLuint* const updatedCurrent	= m_current + 1;

		if (updatedCurrent > m_objectTopBounds) {
			throw std::runtime_error("No more GL objects to Get(). Please Allocate() more objects beforehand");
		}

		DEBUG_EXEC(Debugger::getInstance()->debugEvent(
			DebugEvent::Type::GL_OBJECT_REQUEST,
			std::make_unique<GLMemoryRequest>(m_current, *m_current)));

		m_current = updatedCurrent;

		return *current;
	}

	GLsizeiptr GLAllocator::GetAllocatedObjectCount() const {
		return m_objectTopBounds - m_memory;
	}

	GLsizeiptr GLAllocator::GetLiveObjectCount() const {
		return m_current - m_memory;
	}

	std::string GLAllocator::ToString() const {
		std::stringstream stream;
		
		stream
			<< "GL Allocator - Base: " << std::hex << m_memory
			<< " - Top: " << std::hex << m_memoryTopBounds
			<< " - OpenGL Objects: " << "[";

		stream << std::dec;

		for (auto i = 0; i < GetAllocatedObjectCount(); ++i) {
			stream << *(m_memory + i);

			if (i < GetAllocatedObjectCount() - 1) {
				stream << ", ";
			}
		}

		stream << "]";

		stream << "- Current: " << *m_current;

		return stream.str();
	}

	GLAllocator::~GLAllocator() {
		Free(GetAllocatedObjectCount());

		delete m_memory;
	}
}