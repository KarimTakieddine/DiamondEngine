#include "ComponentFactory.h"
#include "BehaviourComponentConfig.h"
#include "GameInstanceConfig.h"
#include "MaterialRenderComponent.h"
#include "RenderComponentConfig.h"
#include "TransformRenderComponent.h"

namespace
{
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
		// { "Collider2D", ::buildCollider2DComponent }
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

		return result;
	}
}