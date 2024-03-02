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
		GLuint getWidth() const;

		void setWidth(GLuint width);

		GLuint getHeight() const;

		void setHeight(GLuint height);

		ColliderType getColliderType() const;

		void setColliderType(ColliderType colliderType);

		void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) override;

		void OnAddedToScene() override;

		const char* GetName() const override;

	private:
		ColliderType m_colliderType{ ColliderType::OBSTACLE };
		GLuint m_width{ 0 };
		GLuint m_height{ 0 };
	};
}
