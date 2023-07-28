#pragma once

#include "config/SceneConfig.h"

namespace pugi { class xml_document; }

namespace diamond_engine {
class SceneConfigParser {
public:
	static std::unique_ptr<SceneConfig> Parse(const pugi::xml_document& sceneDocument);
};
}
