#pragma once

namespace diamond_engine
{
	class BehaviourComponentConfig
	{
	public:
		virtual ~BehaviourComponentConfig() = default;
		virtual const char* getName() const = 0;
	};
}
