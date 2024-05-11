#include <filesystem>
#include <stdexcept>

#include <pugixml.hpp>

#include "ControllerConfigParser.h"
#include "EngineConfigParser.h"
#include "KeyboardConfigParser.h"
#include "WindowConfigParser.h"

namespace diamond_engine {
	/* static */ EngineConfig EngineConfigParser::Parse(const pugi::xml_node& engineConfigNode) {
		EngineConfig result{
			//KeyboardConfigParser::Parse(engineConfigNode.child("Keyboard")),
			//ControllerConfigParser::Parse(engineConfigNode.child("Controller")),
			WindowConfigParser::Parse(engineConfigNode.child("Window"))
		};

		pugi::xml_attribute shadersDirectoryAttribute = engineConfigNode.attribute("shadersDirectory");
		if (!shadersDirectoryAttribute)
		{
			throw std::runtime_error("Failed to parse EngineConfig. No root \"shadersDirectory\" value was set");
		}
		result.setShadersDirectory(shadersDirectoryAttribute.as_string());

		pugi::xml_attribute texturesDirectoryAttribute = engineConfigNode.attribute("texturesDirectory");
		if (!texturesDirectoryAttribute)
		{
			throw std::runtime_error("Failed to parse EngineConfig. No root \"texturesDirectory\" value was set");
		}
		result.setTexturesDirectory(texturesDirectoryAttribute.as_string());

		return result;
	}
	
	/* static */ EngineConfig EngineConfigParser::Parse(const pugi::xml_document& engineDocument) {
		pugi::xml_node engineNode = engineDocument.root().first_child();

		if (!engineNode) {
			throw std::runtime_error("No root <Engine/> node specified");
		}

		const std::string engineNodeName(engineNode.name());

		if (engineNodeName != "Engine") {
			throw std::runtime_error("Invalid <Engine/> node name found: " + engineNodeName);
		}

		return Parse(engineNode);
	}

	/* static */ EngineConfig EngineConfigParser::ParseFromFile(const std::string& file) {
		const std::filesystem::path path(file);

		if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
			throw std::runtime_error("File not found: " + file);
		}

		pugi::xml_document engineDocument;
		pugi::xml_parse_result parseResult = engineDocument.load_file(path.c_str(), pugi::parse_default, pugi::encoding_utf8);

		if (!parseResult) {
			throw std::runtime_error("Could not load engine config document. Error was: " + std::string(parseResult.description()));
		}

		return Parse(engineDocument);
	}
}