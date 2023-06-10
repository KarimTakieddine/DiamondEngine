#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include "Component.h"
#include "Shader.h"

namespace diamond_engine {
class Material : public Component {
public:
	static const std::string kColorUniformLocation;

	void SetFragmentShader(const std::shared_ptr<Shader> fragmentShader);

	const glm::vec3& GetColor() const;

	void SetColor(const glm::vec3& color);

	void BindToContext() override;

	void Render() override { }

	void Update(GLfloat deltaTime) override;

	const char* GetName() const override {
		return "Material";
	}

protected:
	void OnSetGameObject() override;

	void OnGameObjectAboutToBeUnset() override;

private:
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
};
}
