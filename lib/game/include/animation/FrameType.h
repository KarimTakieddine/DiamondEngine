#pragma once

#include <type_traits>

#include <GL/glew.h>

namespace diamond_engine
{
	enum class FrameType : GLushort
	{
		NONE				= 0,
		LINKABLE			= 1,
		CANCELLABLE			= 1 << 1,
		SUPER_CANCELLABLE	= 1 << 2,
		STARTUP				= 1 << 3,
		ACTIVE				= 1 << 4,
		RECOVERY			= 1 << 5,
		HIT_STUN			= 1 << 6,
		BLOCK_STUN			= 1 << 7
	};

	inline FrameType operator|(FrameType a, FrameType b) { return static_cast<FrameType>( static_cast<std::underlying_type_t<FrameType>>(a) | static_cast<std::underlying_type_t<FrameType>>(b) ); }
	inline FrameType operator&(FrameType a, FrameType b) { return static_cast<FrameType>( static_cast<std::underlying_type_t<FrameType>>(a) & static_cast<std::underlying_type_t<FrameType>>(b) ); }
	inline FrameType& operator|=(FrameType& l, FrameType r) { l = l | r; return l; }
	inline FrameType& operator&=(FrameType& l, FrameType r) { l = l & r; return l; }
	inline FrameType operator~(FrameType t) { return static_cast<FrameType>(~( static_cast<std::underlying_type_t<FrameType>>(t) ) ); }
	inline bool isSet(FrameType f, FrameType t) { return ( f & t ) == t; }
}
