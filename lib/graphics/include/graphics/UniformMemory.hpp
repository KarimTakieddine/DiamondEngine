#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace diamond_engine
{
	template<typename T>
	struct UniformMemory
	{
		T value{ };
		GLsizei count{ 0 };
		GLint location{ -1 };
	};

	struct UniformVec2
	{
		UniformMemory<glm::vec2> memory;
	};

	struct UniformVec3
	{
		UniformMemory<glm::vec3> memory;
		GLubyte padding[4];
	};

	struct UniformMat4
	{
		UniformMemory<glm::mat4> memory;
	};

	inline void uploadUniformMemory(const UniformVec2* uniform)
	{
		const auto& memory = uniform->memory;
		glUniform2fv(memory.location, memory.count, glm::value_ptr(memory.value));
	}

	inline void uploadUniformMemory(const UniformVec3* uniform)
	{
		const auto& memory = uniform->memory;
		glUniform3fv(memory.location, memory.count, glm::value_ptr(memory.value));
	}

	inline void uploadUniformMemory(const UniformMat4* uniform)
	{
		const auto& memory = uniform->memory;
		glUniformMatrix4fv(memory.location, memory.count, GL_FALSE, glm::value_ptr(memory.value));
	}
}
