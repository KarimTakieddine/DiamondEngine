#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "GLAllocator.h"

namespace diamond_engine {
class GameObjectConfig;
class ComponentConfig;

class GameObjectBuilder {
public:
	using ComponentBuildFunc = std::function<std::unique_ptr<Component>(const std::shared_ptr<GLAllocator>&, const ComponentConfig*)>;

	static std::unordered_map<std::string, ComponentBuildFunc> StringToComponentMap;

	static std::unique_ptr<GameObject> Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const GameObjectConfig* gameObjectConfig);
};
}
