#include "GameObject.h"
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

	void Material::BindToContext() {
		m_colorUniformLocation = glGetUniformLocation(
			m_gameObject->GetShaderProgram()->GetObject(),
			kColorUniformLocation.c_str());
	}

	void Material::OnSetGameObject() {
		m_gameObject->GetShaderProgram()->AttachShader(m_fragmentShader);
	}

	void Material::OnGameObjectAboutToBeUnset() {
		m_gameObject->GetShaderProgram()->DetachShader(m_fragmentShader);
	}

	void Material::Update(GLfloat deltaTime) {
		glUniform3f(m_colorUniformLocation, m_color.r, m_color.g, m_color.b);
	}
}