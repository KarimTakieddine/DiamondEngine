#include <stdexcept>

#include "GameObjectBuilder.h"
#include "GameObjectConfig.h"
#include "MeshRendererBuilder.h"

#include "RotateBehaviour.h"

namespace diamond_engine {
	/* static */ std::unordered_map<std::string, GameObjectBuilder::ComponentBuildFunc> GameObjectBuilder::StringToComponentMap = {
		{ "MeshRenderer", &MeshRendererBuilder::Build }
	};

	/* static */ std::unordered_map<std::string, GameObjectBuilder::BehaviourBuildFunc> GameObjectBuilder::StringToBehaviourMap = { };

	/* static */ std::unique_ptr<GameObject> GameObjectBuilder::Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const GameObjectConfig* gameObjectConfig) {
		if (!bufferAllocator) {
			throw std::runtime_error("Null or no buffer allocator set on GameObjectBuilder instance");
		}

		std::unique_ptr<GameObject> result = std::make_unique<GameObject>();

		for (const auto& componentConfig : gameObjectConfig->GetComponentConfigs()) {
			auto componentBuildIt = StringToComponentMap.find(componentConfig->GetName());

			if (componentBuildIt == StringToComponentMap.end()) {
				throw std::runtime_error("Unrecognized component config: " + std::string(componentConfig->GetName()));
			}

			result->AcquireComponent(componentBuildIt->second(bufferAllocator, componentConfig.get()));
		}

		for (const auto& behaviourConfig : gameObjectConfig->GetBehaviourConfigs()) {
			auto behaviourBuildIt = StringToBehaviourMap.find(behaviourConfig->GetName());

			if (behaviourBuildIt == StringToBehaviourMap.end()) {
				throw std::runtime_error("Unrecognized behaviour config: " + std::string(behaviourConfig->GetName()));
			}

			result->AddBehaviour(behaviourBuildIt->second(behaviourConfig.get()));
		}

		return result;
	}

	/* static */ void GameObjectBuilder::RegisterBehaviour(const std::string& name, BehaviourBuildFunc behaviourBuildFunc) {
		StringToBehaviourMap.emplace(name, behaviourBuildFunc);
	}
}