#pragma once

#include <memory>

#include "AlignedAllocator.hpp"
#include "EngineStatus.h"
#include "RenderObject.h"

namespace diamond_engine
{
	class GameInstance;
	class GameInstanceConfig;
	class GameInstanceCreator
	{
	public:
		GameInstanceCreator();

		std::unique_ptr<GameInstance> create(EngineStatus* outStatus = nullptr);
		std::unique_ptr<GameInstance> create(const GameInstanceConfig* instanceConfig, EngineStatus* outStatus = nullptr);

	private:
		std::unique_ptr<AlignedAllocator<RenderObject, 4>> m_renderObjectStore{ nullptr };
	};
}
