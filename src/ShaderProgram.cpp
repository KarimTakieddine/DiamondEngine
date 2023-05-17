#include <algorithm>

#include "ShaderProgram.h"

namespace diamond_engine {
	ShaderProgram::ShaderProgram() : m_shaders(), m_object(glCreateProgram()) { }
	
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
	
	ShaderProgram::~ShaderProgram() {
		for (const auto& shader : m_shaders) {
			glDetachShader(m_object, shader->GetObject());
		}

		m_shaders.clear();

		glDeleteProgram(m_object);
	}
}