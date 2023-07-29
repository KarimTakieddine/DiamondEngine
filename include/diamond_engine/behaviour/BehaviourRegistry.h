#pragma once

#include "GameObjectBuilder.h"
#include "GameObjectConfigParser.h"

namespace diamond_engine {
void RegisterBehaviour(const std::string& name, GameObjectConfigParser::BehaviourParseFunc parseFunc, GameObjectBuilder::BehaviourBuildFunc buildFunc);
}
