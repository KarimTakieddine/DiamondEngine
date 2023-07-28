#pragma once

#include "builder/GameObjectBuilder.h"
#include "parser/GameObjectConfigParser.h"

namespace diamond_engine {
void RegisterBehaviour(const std::string& name, GameObjectConfigParser::BehaviourParseFunc parseFunc, GameObjectBuilder::BehaviourBuildFunc buildFunc);
}
