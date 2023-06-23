#include <stdexcept>
#include <unordered_map>

#include <pugixml.hpp>

#include "MeshRendererConfigParser.h"
#include "MeshRendererConfig.h"

namespace diamond_engine {
	static const std::unordered_map<std::string, MeshType> kStringToMeshType = {
		{ "triangle", MeshType::TRIANGLE },
		{ "quad", MeshType::QUAD }
	};

	static const std::unordered_map<std::string, GLenum> kStringToDrawMode = {
		{ "static", GL_STATIC_DRAW },
		{ "dynamic", GL_DYNAMIC_DRAW }
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
		auto meshTypeIt = kStringToMeshType.find(meshTypeString);
		if (meshTypeIt == kStringToMeshType.cend()) {
			throw std::runtime_error("Unknown \"meshType\" found on MeshRenderer component: " + meshTypeString);
		}

		pugi::xml_attribute drawModeAttribute = componentNode.attribute("drawMode");

		if (!drawModeAttribute) {
			throw std::runtime_error("No \"drawMode\" attribute was found on <MeshRenderer/> node");
		}

		const std::string drawModeString(drawModeAttribute.as_string());

		auto drawModeIt = kStringToDrawMode.find(drawModeString);
		if (drawModeIt == kStringToDrawMode.cend()) {
			throw std::runtime_error("Unknown \"drawMode\" found on MeshRenderer component: " + drawModeString);
		}

		std::unique_ptr<MeshRendererConfig> meshRendererConfig = std::make_unique<MeshRendererConfig>();
		meshRendererConfig->SetMeshType(meshTypeIt->second);
		meshRendererConfig->SetDrawMode(drawModeIt->second);

		return meshRendererConfig;
	}
}
