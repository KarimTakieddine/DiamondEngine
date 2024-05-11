#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include "EngineStatus.h"
#include "GLAllocator.h"

namespace diamond_engine
{
	struct UniformSegment
	{
		const void* data;
		GLsizeiptr stride;
		GLintptr offset;
	};

	struct UniformBuffer
	{
		std::vector<UniformSegment> segments;

		GLuint buffer	{ 0 };
		GLuint binding	{ 0 }; // GL_MAX_UNIFORM_BUFFER_BINDINGS
		GLenum usage	{ GL_STATIC_DRAW };
	};

	class ShaderProgram;
	class UniformBufferAgent
	{
	public:
		UniformBufferAgent();

		inline GLsizeiptr getSizeInBytes(GLint uniformBufferType)
		{
			switch (uniformBufferType)
			{
			case(GL_FLOAT_MAT4):
				return sizeof(glm::mat4);
			default:
				return 0;
			}
		}

		void reserveCapacity(GLsizei capacity);
		void allocateBuffers(GLsizei count);
		void freeBuffers(GLsizei count);
		void freeAllBuffers();
		UniformBuffer allocateBuffer(GLuint binding, GLenum usage);
		void registerUniformBuffer(const UniformBuffer& uniformBuffer);
		EngineStatus bindUniformBuffer(const UniformBuffer& uniformBuffer, const std::shared_ptr<ShaderProgram>& shaderProgram, const std::string& name, const std::vector<const char*>& names);
		EngineStatus uploadBufferData(const UniformBuffer& uniformBuffer);

	private:
		std::unique_ptr<GLAllocator> m_bufferAllocator{ nullptr };
	};
}
