#pragma once

#include <memory>

namespace diamond_engine
{
	class DebugEventData;
	class DebugEvent
	{
	public:
		// Limited to 0xFF
		enum class Type : unsigned char
		{
			UNKNOWN					= 0,
			GL_OBJECT_ALLOCATION	= 1,
			GL_OBJECT_REQUEST		= 2,
			GL_OBJECT_RELEASE		= 3,
			GL_OBJECT_FREE			= 4,
			MEMORY_POOL_ALLOCATION	= 5,
			MEMORY_POOL_REQUEST		= 6,
			MEMORY_POOL_RELEASE		= 7,
			MEMORY_POOL_FREE		= 8,
			FUNCTION_CALL			= 9,
			// No touchy :)
			FINAL_RESERVED			= 0xFF
		};

		virtual ~DebugEvent() = default;
	};
}
