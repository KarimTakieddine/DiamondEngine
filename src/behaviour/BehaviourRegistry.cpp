#include "behaviour/BehaviourRegistry.h"

namespace diamond_engine {
void RegisterBehaviour(const std::string& name, GameObjectConfigParser::BehaviourParseFunc parseFunc, GameObjectBuilder::BehaviourBuildFunc buildFunc) {
	GameObjectConfigParser::RegisterBehaviourConfig(name, parseFunc);
	GameObjectBuilder::RegisterBehaviour(name, buildFunc);
}
}