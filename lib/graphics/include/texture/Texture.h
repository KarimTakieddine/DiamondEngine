#pragma once

#include <GL/glew.h>

namespace diamond_engine {
	struct Texture {
		GLint index{ 0 };
		GLint width{ 0 };
		GLint height{ 0 };
		GLenum wrapModeS{ GL_REPEAT };
		GLenum wrapModeT{ GL_REPEAT };
		GLenum minFilter{ GL_NEAREST };
		GLenum magFilter{ GL_NEAREST };
		GLenum format{ GL_RGB };
	};
}
