#pragma once

#include <GL/glew.h>

#include "RenderUploadType.h"

namespace diamond_engine
{
	struct RenderUpload
	{
		const void* data{ nullptr };
		RenderUploadType type{ RenderUploadType::Unknown };
		GLint location{ -1 };
	};
}
