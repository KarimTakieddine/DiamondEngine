#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "EngineStatus.h"
#include "IRenderComponent.h"

namespace diamond_engine
{
	class RenderComponentConfig;

	class ComponentBuilder
	{
	public:
		using RenderBuildMethod = std::function<std::unique_ptr<IRenderComponent>(const RenderComponentConfig*, EngineStatus*)>;

		static void registerRenderComponent(const std::string& name, RenderBuildMethod buildMethod);

		static std::unique_ptr<IRenderComponent> buildRenderComponent(const RenderComponentConfig* config, EngineStatus* outStatus = nullptr);
	
	private:
		static std::unordered_map<std::string, RenderBuildMethod> renderBuildMethods;
	};
}
