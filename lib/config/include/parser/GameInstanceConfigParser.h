#pragma once

#include <memory>

#include "EngineStatus.h"
#include "GameInstanceConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	std::unique_ptr<GameInstanceConfig> parseGameInstanceConfig(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);
}
