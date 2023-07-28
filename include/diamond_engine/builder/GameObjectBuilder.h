#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "GLAllocator.h"

namespace diamond_engine {
class GameObjectConfig;
class ComponentConfig;
class BehaviourConfig;

class GameObjectBuilder {
public:
	using ComponentBuildFunc = std::function<std::unique_ptr<Component>(const std::shared_ptr<GLAllocator>&, const ComponentConfig*)>;
	using BehaviourBuildFunc = std::function<std::unique_ptr<Behaviour>(const BehaviourConfig*)>;

	static std::unordered_map<std::string, ComponentBuildFunc> StringToComponentMap;

	static std::unordered_map<std::string, BehaviourBuildFunc> StringToBehaviourMap;

	static std::unique_ptr<GameObject> Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const GameObjectConfig* gameObjectConfig);

	static void RegisterBehaviour(const std::string& name, BehaviourBuildFunc behaviourBuildFunc);
};
}
