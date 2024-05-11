#include <stdexcept>

#include "ShaderProgram.h"
#include "UniformBufferAgent.h"

namespace diamond_engine
{
	UniformBufferAgent::UniformBufferAgent() :
		m_bufferAllocator(std::make_unique<GLAllocator>(glGenBuffers, glDeleteBuffers)) { }

	void UniformBufferAgent::reserveCapacity(GLsizei capacity)
	{
		if (capacity < 0)
			throw std::runtime_error("Cannot reserve negative uniform buffer capacity");

		m_bufferAllocator->Reserve(capacity);
	}

	void UniformBufferAgent::allocateBuffers(GLsizei count)
	{
		m_bufferAllocator->Allocate(count);
	}

	void UniformBufferAgent::freeBuffers(GLsizei count)
	{
		m_bufferAllocator->Free(count);
	}

	void UniformBufferAgent::freeAllBuffers()
	{
		freeBuffers(m_bufferAllocator->GetAllocatedObjectCount());
	}

	UniformBuffer UniformBufferAgent::allocateBuffer(GLuint binding, GLenum usage)
	{
		UniformBuffer result;

		result.buffer	= m_bufferAllocator->Get();
		result.binding	= binding;
		result.usage	= usage;

		return result;
	}

	void UniformBufferAgent::registerUniformBuffer(const UniformBuffer& uniformBuffer)
	{
		GLsizeiptr bufferSize = 0;

		for (const auto& segment : uniformBuffer.segments)
		{
			bufferSize += segment.stride;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer.buffer);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, uniformBuffer.usage);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, uniformBuffer.binding, uniformBuffer.buffer, 0, bufferSize);
	}

	EngineStatus UniformBufferAgent::bindUniformBuffer(const UniformBuffer& uniformBuffer, const std::shared_ptr<ShaderProgram>& shaderProgram, const std::string& name, const std::vector<const char*>& names)
	{
		if (!shaderProgram)
		{
			return { "Failed to build uniform buffer. Shader program was null", true };
		}

		const GLuint uniformBlockIndex = glGetUniformBlockIndex(shaderProgram->GetObject(), name.c_str());

		if (uniformBlockIndex == GL_INVALID_INDEX)
		{
			return { "Failed to bind uniform buffer. Could not find block named: " + name, true };
		}

		GLint uniformCount = 0;
		glGetActiveUniformBlockiv(shaderProgram->GetObject(), uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

		if (uniformCount <= 0)
		{
			return { "Failed to bind uniform buffer. Uniform count was zero or negative", true };
		}

		const size_t segmentCount = uniformBuffer.segments.size();
		if (uniformCount != static_cast<GLsizei>(segmentCount))
		{
			return { "Failed to bind uniform buffer. Segment count differs from shader uniform count", true };
		}

		std::vector<GLint> offsets(segmentCount, 0);
		std::vector<GLint> sizes(segmentCount, 0);
		std::vector<GLint> types(segmentCount, 0);

		std::vector<GLuint> uniformIndices(uniformCount, 0);
		glGetUniformIndices(shaderProgram->GetObject(), uniformCount, names.data(), uniformIndices.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_OFFSET, offsets.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_SIZE, sizes.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_TYPE, types.data());

		const std::vector<UniformSegment>& segments = uniformBuffer.segments;
		for (size_t i = 0; i < segmentCount; ++i)
		{
			const UniformSegment& segment = segments[i];

			if (offsets[i] != segment.offset)
			{
				return { "Failed to bind uniform buffer. Segment offset differs from shader uniform", true };
			}

			if (sizes[i] * getSizeInBytes(types[i]) != segment.stride)
			{
				return { "Failed to bind uniform buffer. Segment stride differs from shader uniform", true };
			}
		}

		glUniformBlockBinding(shaderProgram->GetObject(), uniformBlockIndex, uniformBuffer.binding);

		return { };
	}

	EngineStatus UniformBufferAgent::uploadBufferData(const UniformBuffer& uniformBuffer)
	{
		for (const auto& segment : uniformBuffer.segments)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer.buffer);
			glBufferSubData(GL_UNIFORM_BUFFER, segment.offset, segment.stride, segment.data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		return { };
	}
}