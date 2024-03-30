#pragma once

namespace diamond_engine
{
	enum class GameInstanceType
	{
		UNKNOWN = 0,
		SPRITE	= 1
	};

	class GameInstanceConfig
	{
	public:
		virtual ~GameInstanceConfig() = default;
		virtual GameInstanceType getType() const = 0;
	};
}
