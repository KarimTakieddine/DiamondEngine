#pragma once

#include "RenderMaterial.h"
#include "Transform.h"

namespace diamond_engine
{
	struct RenderObject
	{
		Transform transform;
		RenderMaterial material;
	};
}
