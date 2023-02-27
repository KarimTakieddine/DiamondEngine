#include "Material.h"

namespace diamond_engine {
	/* static */ const std::string Material::kColorUniformLocation = "materialColor";

	void Material::SetFragmentShader(const std::shared_ptr<Shader> fragmentShader) {
		m_fragmentShader = fragmentShader;
	}

	const glm::vec3& Material::GetColor() const {
		return m_color;
	}

	void Material::SetColor(const glm::vec3& color) {
		m_color = color;
	}

	void Material::AttachToProgram(ShaderProgram* shaderProgram) {
		shaderProgram->AttachShader(m_fragmentShader);
	}

	void Material::DetachFromProgram(ShaderProgram* shaderProgram) {
		shaderProgram->DetachShader(m_fragmentShader);
	}

	void Material::BindToProgram(const ShaderProgram* shaderProgram) {
		m_colorUniformLocation = glGetUniformLocation(shaderProgram->GetObject(), kColorUniformLocation.c_str());
	}

	void Material::Update() {
		glUniform3f(m_colorUniformLocation, m_color.r, m_color.g, m_color.b);
	}
}