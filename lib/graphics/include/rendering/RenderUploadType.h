#pragma once

namespace diamond_engine
{
	enum class RenderUploadType : unsigned
	{
		Unknown = 0,
		Float = 1,
		Double = 2,
		Vector2 = 3,
		Vector3 = 4,
		Matrix4 = 5

		// etc...
	};
}
