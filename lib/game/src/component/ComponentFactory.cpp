#include <algorithm>

#include "ComponentFactory.h"
#include "BehaviourComponentConfig.h"
#include "Collider2DComponent.h"
#include "GameInstanceConfig.h"
#include "MaterialRenderComponent.h"
#include "RenderComponentConfig.h"
#include "SpriteAnimationPlayer.h"
#include "TransformRenderComponent.h"

namespace
{
	using diamond_engine::BehaviourComponentPtr;
	using diamond_engine::RenderComponentPtr;
	
	static RenderComponentPtr createMaterialComponent()
	{
		using diamond_engine::MaterialRenderComponent;
		return std::make_unique<MaterialRenderComponent>();
	}

	static RenderComponentPtr createTransformComponent()
	{
		using diamond_engine::TransformRenderComponent;
		return std::make_unique<TransformRenderComponent>();
	}

	static BehaviourComponentPtr createCollider2DComponent()
	{
		using diamond_engine::Collider2DComponent;
		return std::make_unique<Collider2DComponent>();
	}

	static BehaviourComponentPtr createSpriteAnimationPlayer()
	{
		using diamond_engine::SpriteAnimationPlayer;
		return std::make_unique<SpriteAnimationPlayer>();
	}
}

namespace diamond_engine
{
	/* static */ void ComponentFactory::registerRenderComponent(const std::string& name, RenderCreateMethod method)
	{
		renderCreateMethods.insert({ name, method });
	}

	/* static */ void ComponentFactory::registerBehaviourComponent(const std::string& name, BehaviourCreateMethod method)
	{
		behaviourCreateMethods.insert({ name, method });
	}

	/* static */ std::unordered_map<std::string, ComponentFactory::RenderCreateMethod> ComponentFactory::renderCreateMethods =
	{
		{ "Material",	::createMaterialComponent	},
		{ "Transform",	::createTransformComponent	}
	};

	/* static */ std::unordered_map<std::string, ComponentFactory::BehaviourCreateMethod> ComponentFactory::behaviourCreateMethods =
	{
		{ "Collider2D",				::createCollider2DComponent },
		{ "SpriteAnimationPlayer",	::createSpriteAnimationPlayer }
	};

	/* static */ RenderComponentPtr	ComponentFactory::createRenderComponent(const RenderComponentConfig* config)
	{
		auto methodIt = renderCreateMethods.find(config->getName());
		return methodIt == renderCreateMethods.cend() ? nullptr : methodIt->second();
	}

	/* static */ std::vector<RenderComponentPtr> ComponentFactory::createRenderComponents(const GameInstanceConfig* instanceConfig)
	{
		std::vector<RenderComponentPtr> result;
		for (const auto& renderConfig : instanceConfig->getRenderConfigs())
		{
			result.push_back(createRenderComponent(renderConfig.get()));
		}

		return result;
	}

	/* static */ BehaviourComponentPtr ComponentFactory::createBehaviourComponent(const BehaviourComponentConfig* config)
	{
		auto methodIt = behaviourCreateMethods.find(config->getName());
		return methodIt == behaviourCreateMethods.cend() ? nullptr : methodIt->second();
	}

	/* static */ std::vector<BehaviourComponentPtr>	ComponentFactory::createBehaviourComponents(const GameInstanceConfig* instanceConfig)
	{
		std::vector<BehaviourComponentPtr> result;
		for (const auto& behaviourConfig : instanceConfig->getBehaviourConfigs())
		{
			result.push_back(createBehaviourComponent(behaviourConfig.get()));
		}

		if (instanceConfig->getType() == GameInstanceType::SPRITE)
		{
			auto collider2DIt = std::find_if(
				result.cbegin(),
				result.cend(),
				[](const auto& component) { return std::string(component->getName()) == "Collider2D"; });

			if (collider2DIt != result.cend())
				result.erase(collider2DIt);
		}

		return result;
	}
}