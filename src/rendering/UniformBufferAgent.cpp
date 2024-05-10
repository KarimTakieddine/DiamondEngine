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

	void UniformBufferAgent::bindUniformBuffer(const UniformBuffer& uniformBuffer)
	{
		GLsizeiptr bufferSize = 0;
		const std::vector<GLint>& sizes = uniformBuffer.sizes;
		const std::vector<GLint>& types = uniformBuffer.types;

		for (GLuint i = 0; i < uniformBuffer.count; ++i)
		{
			bufferSize += sizes[i] * getSizeInBytes(types[i]);
		}

		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer.buffer);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, uniformBuffer.usage);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, uniformBuffer.binding, uniformBuffer.buffer, 0, bufferSize);
	}

	EngineStatus UniformBufferAgent::buildUniformBuffer(const std::shared_ptr<ShaderProgram>& shaderProgram, const std::string& name, const std::vector<const char*>& names, UniformBuffer* uniformBuffer)
	{
		if (!shaderProgram)
		{
			return { "Failed to build uniform buffer. Shader program was null", true };
		}

		const GLuint uniformBlockIndex = glGetUniformBlockIndex(shaderProgram->GetObject(), name.c_str());

		if (uniformBlockIndex == GL_INVALID_INDEX)
		{
			return { "Failed to build uniform buffer. Could not find block named: " + name, true };
		}

		if (!uniformBuffer)
		{
			return { "Failed to build uniform buffer. Cannot operate on null buffer", true };
		}

		GLint uniformCount = 0;
		glGetActiveUniformBlockiv(shaderProgram->GetObject(), uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

		if (uniformCount <= 0)
		{
			return { "Failed to build uniform buffer. Uniform count was zero or negative", true };
		}

		uniformBuffer->count = static_cast<GLuint>(uniformCount);
		uniformBuffer->offsets.resize(uniformCount, 0);
		uniformBuffer->sizes.resize(uniformCount, 0);
		uniformBuffer->types.resize(uniformCount, 0);

		std::vector<GLuint> uniformIndices(uniformCount, 0);
		glGetUniformIndices(shaderProgram->GetObject(), uniformCount, names.data(), uniformIndices.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_OFFSET, uniformBuffer->offsets.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_SIZE, uniformBuffer->sizes.data());
		glGetActiveUniformsiv(shaderProgram->GetObject(), uniformCount, uniformIndices.data(), GL_UNIFORM_TYPE, uniformBuffer->types.data());
		glUniformBlockBinding(shaderProgram->GetObject(), uniformBlockIndex, uniformBuffer->binding);

		return { };
	}

	EngineStatus UniformBufferAgent::uploadBufferData(const UniformBuffer& uniformBuffer, const std::vector<const void*>& data)
	{
		if (uniformBuffer.count != data.size())
		{
			return { "Uniform buffer upload failed. Number of uniforms differs from data set count", true };
		}

		const std::vector<GLint>& offsets	= uniformBuffer.offsets;
		const std::vector<GLint>& sizes		= uniformBuffer.sizes;
		const std::vector<GLint>& types		= uniformBuffer.types;

		for (GLuint i = 0; i < uniformBuffer.count; ++i)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer.buffer);
			glBufferSubData(GL_UNIFORM_BUFFER, offsets[i], sizes[i] * getSizeInBytes(types[i]), data[i]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		return { };
	}
}