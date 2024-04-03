#pragma once

namespace diamond_engine
{
	class RenderComponentConfig
	{
	public:
		virtual ~RenderComponentConfig() = default;
		virtual const char* getName() const = 0;
	};
}
