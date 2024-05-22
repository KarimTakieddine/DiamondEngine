#pragma once

#include <GL/glew.h>

namespace diamond_engine {
	enum class MeshType : GLubyte
	{
		TRIANGLE		= 0,
		QUAD			= 1,
		CUBE			= 2,
		COLLIDER		= 3,
		FREE_SLOT_0		= 4,
		FREE_SLOT_1		= 5,
		FREE_SLOT_2		= 6,
		FREE_SLOT_3		= 7,
		FREE_SLOT_4		= 8,
		FREE_SLOT_5		= 9,
		FREE_SLOT_6		= 10,
		FREE_SLOT_7		= 11,
		FREE_SLOT_8		= 12,
		FREE_SLOT_9		= 13,
		FREE_SLOT_10	= 14,
		FREE_SLOT_11	= 15
	};
}
