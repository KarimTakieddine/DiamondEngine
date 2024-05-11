#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "BehaviourComponent.h"
#include "EngineStatus.h"
#include "IRenderComponent.h"

namespace diamond_engine
{
	class RenderComponentConfig;
	class BehaviourComponentConfig;

	class ComponentBuilder
	{
	public:
		using RenderBuildMethod		= std::function<std::unique_ptr<IRenderComponent>(const RenderComponentConfig*, EngineStatus*)>;
		using BehaviourBuildMethod	= std::function<std::unique_ptr<BehaviourComponent>(const BehaviourComponentConfig*, EngineStatus*)>;

		static void registerRenderComponent(const std::string& name, RenderBuildMethod buildMethod);
		static void registerBehaviourComponent(const std::string& name, BehaviourBuildMethod buildMethod);

		static std::unique_ptr<IRenderComponent> buildRenderComponent(const RenderComponentConfig* config, EngineStatus* outStatus = nullptr);
		static std::unique_ptr<BehaviourComponent> buildBehaviourComponent(const BehaviourComponentConfig* config, EngineStatus* outStatus = nullptr);

	private:
		static std::unordered_map<std::string, RenderBuildMethod> renderBuildMethods;
		static std::unordered_map<std::string, BehaviourBuildMethod> behaviourBuildMethods;
	};
}
