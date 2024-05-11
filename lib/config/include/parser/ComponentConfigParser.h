#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "EngineStatus.h"
#include "BehaviourComponentConfig.h"
#include "RenderComponentConfig.h"

namespace pugi { class xml_node; }

namespace diamond_engine
{
	class ComponentConfigParser
	{
	public:
		using RenderParseMethod		= std::function<std::unique_ptr<RenderComponentConfig>(const pugi::xml_node&, EngineStatus*)>;
		using BehaviourParseMethod	= std::function<std::unique_ptr<BehaviourComponentConfig>(const pugi::xml_node&, EngineStatus*)>;

		static void registerRenderConfig(const std::string& name, RenderParseMethod parseMethod);
		static void registerBehaviourConfig(const std::string& name, BehaviourParseMethod parseMethod);

		static std::unique_ptr<RenderComponentConfig> parseRenderComponent(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);
		static std::unique_ptr<BehaviourComponentConfig> parseBehaviourComponent(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);

	private:	
		static std::unordered_map<std::string, RenderParseMethod> renderParseMethods;
		static std::unordered_map<std::string, BehaviourParseMethod> behaviourParseMethods;
	};
}
