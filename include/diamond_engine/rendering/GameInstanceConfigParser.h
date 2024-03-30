#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace pugi { class xml_node; }

namespace diamond_engine
{
	struct EngineStatus;
	class GameInstanceConfig;
	class MaterialConfig;
	class MeshRenderConfig;
	class TransformConfig;
	class GameInstanceConfigParser
	{
	public:
		static std::unique_ptr<GameInstanceConfig> parse(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);

	private:
		static MaterialConfig parseMaterialConfig(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);
		static MeshRenderConfig parseMeshRenderConfig(const pugi::xml_node& node, EngineStatus* outStatus = nullptr);
	};
}
