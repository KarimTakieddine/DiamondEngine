#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "Shader.h"

namespace diamond_engine {
class ShaderProgram {
public:
	ShaderProgram();

	void AttachShader(const std::shared_ptr<Shader>& shader);

	void DetachShader(const std::shared_ptr<Shader>& shader);

	void Link();

	bool IsLinked() const;

	GLuint GetObject() const;

	~ShaderProgram();

private:
	std::vector<std::shared_ptr<Shader>> m_shaders;
	GLuint m_object{ 0 };
	GLint m_linked{ GL_TRUE };
};
}
