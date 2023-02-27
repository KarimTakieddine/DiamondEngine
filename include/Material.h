#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include "Component.h"
#include "ShaderProgram.h"

namespace diamond_engine {
class Material : public Component {
public:
	static const std::string kColorUniformLocation;

	void SetFragmentShader(const std::shared_ptr<Shader> fragmentShader);

	const glm::vec3& GetColor() const;

	void SetColor(const glm::vec3& color);

	void BindToContext() override { }

	void AttachToProgram(ShaderProgram* shaderProgram) override;

	void DetachFromProgram(ShaderProgram* shaderProgram) override;

	void BindToProgram(const ShaderProgram* shaderProgram) override;

	void Render() override { }

	void Update() override;

	const char* GetName() const override {
		return "Material";
	}

private:
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
	std::shared_ptr<Shader> m_fragmentShader{ nullptr };
	GLint m_colorUniformLocation{ -1 };
};
}
