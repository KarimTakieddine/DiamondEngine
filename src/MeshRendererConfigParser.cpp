#include <stdexcept>

#include <pugixml.hpp>

#include "MeshRendererConfigParser.h"
#include "MeshRendererConfig.h"

namespace diamond_engine {
	static const std::unordered_map<std::string, MeshType> kStringToMeshType = {
		{ "triangle", MeshType::TRIANGLE },
		{ "quad", MeshType::QUAD }
	};

	std::unique_ptr<ComponentConfig> MeshRendererConfigParser::Parse(const pugi::xml_node& componentNode) {
		if (!componentNode) {
			throw std::runtime_error("No node given to MeshRendererConfigParser::Parse()");
		}

		pugi::xml_attribute meshTypeAttribute = componentNode.attribute("type");

		if (!meshTypeAttribute) {
			throw std::runtime_error("No \"meshType\" attribute was found on <MeshRenderer/> node");
		}

		const std::string meshTypeString(meshTypeAttribute.as_string());
		auto it = kStringToMeshType.find(meshTypeString);
		if (it == kStringToMeshType.cend()) {
			throw std::runtime_error("Unknown \"meshType\" found on MeshRenderer component: " + meshTypeString);
		}

		std::unique_ptr<MeshRendererConfig> meshRendererConfig = std::make_unique<MeshRendererConfig>();
		meshRendererConfig->SetMeshType(it->second);

		return meshRendererConfig;
	}
}
