#pragma once

#include <type_traits>

#include <GL/glew.h>

namespace diamond_engine
{
	enum class ColliderIgnoreFlags : GLbyte
	{
		NONE	= 0,
		UP		= 1,
		DOWN	= 1 << 1,
		LEFT	= 1 << 2,
		RIGHT	= 1 << 3
	};

	inline ColliderIgnoreFlags operator|(ColliderIgnoreFlags lhs, ColliderIgnoreFlags rhs)
	{
		return static_cast<ColliderIgnoreFlags>(
			static_cast<std::underlying_type_t<ColliderIgnoreFlags>>(lhs) |
			static_cast<std::underlying_type_t<ColliderIgnoreFlags>>(rhs));
	}

	inline ColliderIgnoreFlags& operator|=(ColliderIgnoreFlags& lhs, ColliderIgnoreFlags rhs)
	{
		lhs = lhs | rhs;

		return lhs;
	}

	inline ColliderIgnoreFlags operator&(ColliderIgnoreFlags lhs, ColliderIgnoreFlags rhs)
	{
		return static_cast<ColliderIgnoreFlags>(
			static_cast<std::underlying_type_t<ColliderIgnoreFlags>>(lhs) &
			static_cast<std::underlying_type_t<ColliderIgnoreFlags>>(rhs));
	}

	inline ColliderIgnoreFlags& operator&=(ColliderIgnoreFlags& lhs, ColliderIgnoreFlags rhs)
	{
		lhs = lhs & rhs;

		return lhs;
	}

	inline ColliderIgnoreFlags operator~(ColliderIgnoreFlags f)
	{
		return static_cast<ColliderIgnoreFlags>( ~( static_cast<std::underlying_type_t<ColliderIgnoreFlags>>(f) ) );
	}
}
