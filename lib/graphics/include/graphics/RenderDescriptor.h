#pragma once

#include <vector>

#include "UniformDescriptor.h"

namespace diamond_engine
{
	using UniformList = std::vector<UniformDescriptor>;

	struct RenderDescriptor
	{
		UniformList uniforms;
	};
}
