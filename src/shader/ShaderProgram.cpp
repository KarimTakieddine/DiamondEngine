#include <algorithm>
#include <stdexcept>

#include "shader/ShaderProgram.h"

namespace diamond_engine {
	ShaderProgram::ShaderProgram() : m_shaders(), m_object(glCreateProgram()) {
		m_attributes.reserve(GL_MAX_VERTEX_ATTRIBS);
		m_uniforms.reserve(GL_MAX_UNIFORM_LOCATIONS);
	}
	
	void ShaderProgram::AttachShader(const std::shared_ptr<Shader>& shader) {
		m_shaders.push_back(shader);

		glAttachShader(m_object, shader->GetObject());
	}
	
	void ShaderProgram::DetachShader(const std::shared_ptr<Shader>& shader) {
		glDetachShader(m_object, shader->GetObject());

		m_shaders.erase(
			std::remove_if(
				m_shaders.begin(),
				m_shaders.end(),
				[&](const auto& element) { return element->GetObject() == shader->GetObject(); }));
	}

	void ShaderProgram::Link() {
		if (m_shaders.empty()) {
			return;
		}

		glLinkProgram(m_object);
		glGetProgramiv(m_object, GL_LINK_STATUS, &m_linked);
	}

	bool ShaderProgram::IsLinked() const {
		return m_linked;
	}

	GLuint ShaderProgram::GetObject() const {
		return m_object;
	}

	void ShaderProgram::LoadAttributes(const std::vector<std::string>& attributes) {
		LoadVariables(attributes, glGetAttribLocation, m_attributes);
	}

	GLint ShaderProgram::GetAttribute(const std::string& name) const {
		auto it = m_attributes.find(name);
		if (it == m_attributes.cend()) {
			return -1;
		}

		return it->second;
	}

	void ShaderProgram::LoadUniforms(const std::vector<std::string>& uniforms) {
		LoadVariables(uniforms, glGetUniformLocation, m_uniforms);
	}

	GLint ShaderProgram::GetUniform(const std::string& name) const {
		auto it = m_uniforms.find(name);
		if (it == m_uniforms.cend()) {
			return -1;
		}

		return it->second;
	}

	ShaderProgram::~ShaderProgram() {
		for (const auto& shader : m_shaders) {
			glDetachShader(m_object, shader->GetObject());
		}

		m_shaders.clear();

		glDeleteProgram(m_object);
	}

	void ShaderProgram::LoadVariables(const std::vector<std::string>& variables, VarFindFunc findFunc, std::unordered_map<std::string, GLint>& data) {
		if (!findFunc) {
			throw std::runtime_error("No find function supplied to initialize shader program variables");
		}

		for (const auto& variable : variables) {
			const GLint variableLocation = findFunc(m_object, variable.c_str());

			data.emplace(variable, variableLocation);
		}
	}
}