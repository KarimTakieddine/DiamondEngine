#pragma once

#include <memory>
#include <vector>

namespace diamond_engine
{
	class GameInstanceConfig;
	class IRenderComponent;
	class IGameInstanceBuilder
	{
	public:
		virtual ~IGameInstanceBuilder() = default;
		virtual std::vector<std::unique_ptr<IRenderComponent>> getRenderComponents(const GameInstanceConfig* instanceConfig) const = 0;
	};
}
