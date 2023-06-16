#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "Shader.h"

namespace diamond_engine {
class ShaderProgram {
public:
	using VarFindFunc = std::function<GLint(GLuint, const GLchar*)>;

	ShaderProgram();

	void AttachShader(const std::shared_ptr<Shader>& shader);

	void DetachShader(const std::shared_ptr<Shader>& shader);

	void Link();

	bool IsLinked() const;

	GLuint GetObject() const;

	void LoadAttributes(const std::vector<std::string>& attributes);

	GLint GetAttribute(const std::string& name) const;

	void LoadUniforms(const std::vector<std::string>& uniforms);

	GLint GetUniform(const std::string& name) const;

	~ShaderProgram();

private:
	void LoadVariables(
		const std::vector<std::string>& variables,
		VarFindFunc findFunc,
		std::unordered_map<std::string, GLint>& data);

	std::vector<std::shared_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, GLint> m_attributes;
	std::unordered_map<std::string, GLint> m_uniforms;
	GLuint m_object{ 0 };
	GLint m_linked{ GL_TRUE };
};
}
