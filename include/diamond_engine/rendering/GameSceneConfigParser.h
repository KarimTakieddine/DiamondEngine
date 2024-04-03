#pragma once

#include <memory>
#include <vector>

#include "EngineStatus.h"
#include "GameSceneConfig.h"

namespace pugi { class xml_document; class xml_node; }

namespace diamond_engine
{
	std::unique_ptr<GameSceneConfig> parseSceneFile(const std::string& file, EngineStatus* outStatus = nullptr);
	std::unique_ptr<GameSceneConfig> parseSceneDocument(const pugi::xml_document& document, EngineStatus* outStatus = nullptr);
	std::unique_ptr<GameSceneConfig> parseScene(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);
}
