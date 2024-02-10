#pragma once

#include "Component.h"

namespace diamond_engine {
	class Collider2D : public Component
	{
	public:
		GLuint getWidth() const;

		void setWidth(GLuint width);

		GLuint getHeight() const;

		void setHeight(GLuint height);

		void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) override;

		void OnAddedToScene() override;

		const char* GetName() const override;

	private:
		GLuint m_width{ 0 };
		GLuint m_height{ 0 };
	};
}
