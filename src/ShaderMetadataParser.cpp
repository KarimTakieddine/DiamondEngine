#include <stdexcept>

#include "ShaderMetadataParser.h"

namespace diamond_engine {
	/* static */ const ShaderMetadataParser::StringToShaderTypeMap ShaderMetadataParser::kStringToShaderTypeMap = {
		{ "vertex", GL_VERTEX_SHADER },
		{ "fragment", GL_FRAGMENT_SHADER }
	};

	GLenum ShaderMetadataParser::ShaderTypeFromString(const std::string& shaderType) {
		const auto it = kStringToShaderTypeMap.find(shaderType);

		if (it == kStringToShaderTypeMap.cend()) {
			throw std::runtime_error("Unknown shader type specified: " + shaderType);
		}

		return it->second;
	}

	/* static */ ShaderMetadata ShaderMetadataParser::ParseShaderMetadata(const pugi::xml_node& shaderNode)
	{
		ShaderMetadata result;

		pugi::xml_attribute fileAttribute = shaderNode.find_attribute([](const pugi::xml_attribute& attribute) { return std::string(attribute.name()) == "file"; });
		if (!fileAttribute) {
			throw std::runtime_error("Could not find \"file\" attribute on parsed <Shader/> node");
		}
		result.file = std::string(fileAttribute.as_string());

		pugi::xml_attribute typeAttribute = shaderNode.find_attribute([](const pugi::xml_attribute& attribute) { return std::string(attribute.name()) == "type"; });
		if (!typeAttribute) {
			throw std::runtime_error("Could not find \"type\" attribute on parsed <Shader/> node");
		}
		result.type = ShaderTypeFromString(std::string(typeAttribute.as_string()));

		return result;
	}

	/*static */ ShaderProgramMetadata ShaderMetadataParser::ParseShaderProgramMetadata(const pugi::xml_document& programDocument) {
		ShaderProgramMetadata result;

		pugi::xml_node programNode = programDocument.root().first_child();

		const std::string programNodeName(programNode.name());

		if (!programNode || programNodeName != "Program") {
			throw std::runtime_error("Failed to parse shader program document. Could not find node named: \"Program\"");
		}

		pugi::xml_attribute nameAttribute = programNode.find_attribute([](const pugi::xml_attribute& attribute) { return std::string(attribute.name()) == "name"; });
		if (!nameAttribute) {
			throw std::runtime_error("Could not find \"name\" attribute on parsed <Program/> root node");
		}
		result.SetName(std::string(nameAttribute.as_string()));

		for (const auto& shaderNode : programNode.children("Shader")) {
			result.AddShaderData(ParseShaderMetadata(shaderNode));
		}

		return result;
	}

	ShaderProgramMetadata ShaderMetadataParser::LoadShaderProgramMetadata(const std::string& file) {
		pugi::xml_document programDocument;
		pugi::xml_parse_result parseResult = programDocument.load_file(file.c_str(), pugi::parse_default, pugi::xml_encoding::encoding_utf8);
		if (!parseResult) {
			throw std::runtime_error("Failed to parse document at file: " + file + ". Error: " + std::string(parseResult.description()));
		}
		return ParseShaderProgramMetadata(programDocument);
	}

	void ShaderProgramMetadata::AddShaderData(const ShaderMetadata& shaderData) {
		m_shaderData.push_back(shaderData);
	}

	void ShaderProgramMetadata::SetName(const std::string& name) {
		m_name = name;
	}
}