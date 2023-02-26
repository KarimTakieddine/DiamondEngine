#include <stdexcept>
#include <vector>

#include "File.h"
#include "Shader.h"

namespace diamond_engine {
	Shader::Shader(GLenum type) : m_object(glCreateShader(type)) { }

	void Shader::SetSource(const std::string& source) {
		if (source.size() > kMaxSourceLength) {
			throw std::runtime_error(
				"Shader source lenth is too long. Maximum length is: " + std::to_string(kMaxSourceLength));
		}

		const char* sourceCStr = source.c_str();
		glShaderSource(m_object, 1, &sourceCStr, nullptr);
	}

	void Shader::Compile() {
		glCompileShader(m_object);
		glGetShaderiv(m_object, GL_COMPILE_STATUS, &m_compiled);
	}

	bool Shader::Load(const std::string& file) {
		SetSource(File::Read(file));
		Compile();
		return IsCompiled();
	}

	std::string Shader::GetSource() const {
		std::vector<char> sourceBuffer(kMaxSourceLength, '\0');
		glGetShaderSource(m_object, kMaxSourceLength, nullptr, sourceBuffer.data());
		return std::string(sourceBuffer.data());
	}

	bool Shader::IsCompiled() const {
		return m_compiled;
	}

	GLuint Shader::GetObject() const {
		return m_object;
	}

	Shader::~Shader() {
		glDeleteShader(m_object);
	}
}