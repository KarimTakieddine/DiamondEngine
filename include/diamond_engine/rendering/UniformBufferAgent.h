#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include "EngineStatus.h"
#include "GLAllocator.h"

namespace diamond_engine
{
	struct UniformBuffer
	{
		std::vector<GLint> offsets;
		std::vector<GLint> sizes;
		std::vector<GLint> types;

		GLuint buffer	{ 0 };
		GLuint binding	{ 0 }; // GL_MAX_UNIFORM_BUFFER_BINDINGS
		GLuint count	{ 0 };
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
		EngineStatus buildUniformBuffer(const std::shared_ptr<ShaderProgram>& shaderProgram, const std::string& name, const std::vector<const char*>& names, UniformBuffer* uniformBuffer);
		void bindUniformBuffer(const UniformBuffer& uniformBuffer);
		EngineStatus uploadBufferData(const UniformBuffer& uniformBuffer, const std::vector<const void*>&data);

	private:
		std::unique_ptr<GLAllocator> m_bufferAllocator{ nullptr };
	};
}
