#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "BehaviourComponent.h"
#include "IRenderComponent.h"

namespace diamond_engine
{
	using RenderComponentPtr	= std::unique_ptr<IRenderComponent>;
	using BehaviourComponentPtr = std::unique_ptr<BehaviourComponent>;

	class GameInstanceConfig;
	class RenderComponentConfig;
	class BehaviourComponentConfig;
	class ComponentFactory
	{
	public:
		using RenderCreateMethod	= std::function<RenderComponentPtr(void)>;
		using BehaviourCreateMethod	= std::function<BehaviourComponentPtr(void)>;

		static void registerRenderComponent		(const std::string& name, RenderCreateMethod method);
		static void registerBehaviourComponent	(const std::string& name, BehaviourCreateMethod method);

		static RenderComponentPtr					createRenderComponent		(const RenderComponentConfig* config);
		static std::vector<RenderComponentPtr>		createRenderComponents		(const GameInstanceConfig* instanceConfig);
		static BehaviourComponentPtr				createBehaviourComponent	(const BehaviourComponentConfig* config);
		static std::vector<BehaviourComponentPtr>	createBehaviourComponents	(const GameInstanceConfig* instanceConfig);

	private:
		static std::unordered_map<std::string, RenderCreateMethod> renderCreateMethods;
		static std::unordered_map<std::string, BehaviourCreateMethod> behaviourCreateMethods;
	};
}
