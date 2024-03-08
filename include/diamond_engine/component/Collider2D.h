#pragma once

#include "Component.h"

namespace diamond_engine {
	enum class ColliderType : GLuint
	{
		OBSTACLE	= 0,
		CHARACTER	= 1
	};

	class Collider2D : public Component
	{
	public:
		GLfloat getWidth() const;

		void setWidth(GLfloat width);

		GLfloat getHeight() const;

		void setHeight(GLfloat height);

		ColliderType getColliderType() const;

		void setColliderType(ColliderType colliderType);

		void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) override;

		void OnAddedToScene() override;

		const char* GetName() const override;

	private:
		ColliderType m_colliderType{ ColliderType::OBSTACLE };
		GLfloat m_width{ 0.0f };
		GLfloat m_height{ 0.0f };
	};
}
