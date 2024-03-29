#pragma once

namespace diamond_engine
{
	class InstanceConfig
	{
	public:
		virtual ~InstanceConfig() = default;
		virtual const char* getName() const = 0;
	};
}
