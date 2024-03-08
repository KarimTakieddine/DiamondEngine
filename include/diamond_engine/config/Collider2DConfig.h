#pragma once

#include <GL/glew.h>

#include "Collider2D.h"
#include "ComponentConfig.h"

namespace diamond_engine {
	class Collider2DConfig : public ComponentConfig {
	public:
		virtual ~Collider2DConfig() override = default;

		GLfloat getWidth() const;

		void setWidth(GLfloat width);

		GLfloat getHeight() const;

		void setHeight(GLfloat height);

		ColliderType getColliderType() const;

		void setColliderType(ColliderType colliderType);

		const char* GetName() const override {
			return "Collider2D";
		}

	private:
		ColliderType m_colliderType{ ColliderType::OBSTACLE };
		GLfloat m_width{ 0 };
		GLfloat m_height{ 0 };
	};
}
