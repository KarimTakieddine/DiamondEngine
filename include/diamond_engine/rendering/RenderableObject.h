#pragma once

#include "DrawCall.h"
#include "Material.h"
#include "Transform.h"

namespace diamond_engine {
	struct RenderableObject {
		Transform transform{ };
		Material material{ };
		DrawCall drawCall{ };
		GLuint vertexArrayObject{ 0 };
		GLint objectModelUniformLocation{ -1 };
		GLint colorUniformLocation{ -1 };
		GLint textureOffsetUniformLocation{ -1 };
	};
}
