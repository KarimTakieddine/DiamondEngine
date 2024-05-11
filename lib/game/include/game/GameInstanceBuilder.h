#pragma once

#pragma once

#include <memory>

#include "AlignedAllocator.hpp"
#include "EngineStatus.h"
#include "RenderObject.h"

namespace diamond_engine
{
	class GameInstance;
	class GameInstanceConfig;
	
	EngineStatus buildGameInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig);
}
