#include <filesystem>
#include <stdexcept>

#include <pugixml.hpp>

#include "ControllerConfigParser.h"
#include "EngineConfigParser.h"
#include "KeyboardConfigParser.h"
#include "WindowConfigParser.h"

namespace diamond_engine {
	/* static */ EngineConfig EngineConfigParser::Parse(const pugi::xml_node& engineConfigNode) {
		return {
			KeyboardConfigParser::Parse(engineConfigNode.child("Keyboard")),
			ControllerConfigParser::Parse(engineConfigNode.child("Controller")),
			WindowConfigParser::Parse(engineConfigNode.child("Window"))
		};
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