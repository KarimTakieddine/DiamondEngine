#include <stdexcept>

#include "Collider2DBuilder.h"
#include "GameObjectBuilder.h"
#include "GameObjectConfig.h"
#include "MeshRendererBuilder.h"
#include "RotateBehaviourBuilder.h"
#include "ScrollingBackgroundBehaviourBuilder.h"
#include "SpriteAnimationPlayerBuilder.h"

namespace diamond_engine {
	/* static */ std::unordered_map<std::string, GameObjectBuilder::ComponentBuildFunc> GameObjectBuilder::StringToComponentMap = {
		{ "Collider2D",				&Colllider2DBuilder::Build },
		{ "MeshRenderer",			&MeshRendererBuilder::Build },
		{ "SpriteAnimationPlayer",	&SpriteAnimationPlayerBuilder::Build }
	};

	/* static */ std::unordered_map<std::string, GameObjectBuilder::BehaviourBuildFunc> GameObjectBuilder::StringToBehaviourMap = {
		{ "RotateBehaviour",				&RotateBehaviourBuilder::Build },
		{ "ScrollingBackgroundBehaviour",	&ScrollingBackgroundBehaviourBuilder::Build }
	};

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