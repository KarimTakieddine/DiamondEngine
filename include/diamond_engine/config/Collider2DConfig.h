#pragma once

#include <GL/glew.h>

#include "ComponentConfig.h"

namespace diamond_engine {
	class Collider2DConfig : public ComponentConfig {
	public:
		virtual ~Collider2DConfig() override = default;

		GLuint getWidth() const;

		void setWidth(GLuint width);

		GLuint getHeight() const;

		void setHeight(GLuint height);

		const char* GetName() const override {
			return "Collider2D";
		}

	private:
		GLuint m_width{ 0 };
		GLuint m_height{ 0 };
	};
}
