#include <pugixml.hpp>

#include <stdexcept>

#include "ShaderConfigParser.h"

namespace diamond_engine {
	/* static */ const ShaderConfigParser::StringToShaderTypeMap ShaderConfigParser::kStringToShaderTypeMap = {
		{ "vertex", GL_VERTEX_SHADER },
		{ "fragment", GL_FRAGMENT_SHADER }
	};

	GLenum ShaderConfigParser::ShaderTypeFromString(const std::string& shaderType) {
		const auto it = kStringToShaderTypeMap.find(shaderType);

		if (it == kStringToShaderTypeMap.cend()) {
			throw std::runtime_error("Unknown shader type specified: " + shaderType);
		}

		return it->second;
	}

	/* static */ ShaderConfig ShaderConfigParser::ParseShaderConfig(const pugi::xml_node& shaderNode)
	{
		ShaderConfig result;

		pugi::xml_attribute fileAttribute = shaderNode.attribute("file");
		if (!fileAttribute) {
			throw std::runtime_error("Could not find \"file\" attribute on parsed <Shader/> node");
		}

		const std::string file(fileAttribute.as_string());

		if (file.empty()) {
			throw std::runtime_error("Cannot set empty \"file\" attribute on parsed <Shader/> node");
		}

		result.file = file;

		pugi::xml_attribute typeAttribute = shaderNode.attribute("type");
		if (!typeAttribute) {
			throw std::runtime_error("Could not find \"type\" attribute on parsed <Shader/> node");
		}
		result.type = ShaderTypeFromString(std::string(typeAttribute.as_string()));

		pugi::xml_node attributesNode = shaderNode.child("Attributes");
		for (pugi::xml_node attributeNode : attributesNode.children("Attribute")) {
			pugi::xml_attribute nameAttribute = attributeNode.attribute("name");

			if (!nameAttribute) {
				throw std::runtime_error("No \"name\" attribute specified for <Attribute/> node child of parsed <Shader/> node");
			}

			result.attributes.push_back(std::string(nameAttribute.as_string()));
		}

		pugi::xml_node uniformsNode = shaderNode.child("Uniforms");
		for (pugi::xml_node uniformNode : uniformsNode.children("Uniform")) {
			pugi::xml_attribute nameAttribute = uniformNode.attribute("name");

			if (!nameAttribute) {
				throw std::runtime_error("No \"name\" attribute specified for <Uniform/> node child of parsed <Shader/> node");
			}

			result.uniforms.push_back(std::string(nameAttribute.as_string()));
		}

		return result;
	}

	/*static */ ShaderProgramConfig ShaderConfigParser::ParseProgramConfig(const pugi::xml_document& programDocument) {
		ShaderProgramConfig result;

		pugi::xml_node programNode = programDocument.root().first_child();

		const std::string programNodeName(programNode.name());

		if (!programNode || programNodeName != "Program") {
			throw std::runtime_error("Failed to parse shader program document. Could not find node named: \"Program\"");
		}

		pugi::xml_attribute nameAttribute = programNode.attribute("name");
		if (!nameAttribute) {
			throw std::runtime_error("Could not find \"name\" attribute on parsed <Program/> root node");
		}

		const std::string name(nameAttribute.as_string());
		if (name.empty()) {
			throw std::runtime_error("Cannot set empty \"name\" attribute on parsed <Program/> root node");
		}
		result.SetName(name);

		for (const auto& shaderNode : programNode.children("Shader")) {
			result.AddShaderConfig(ParseShaderConfig(shaderNode));
		}

		return result;
	}

	ShaderProgramConfig ShaderConfigParser::ParseProgramConfig(const std::string& file) {
		pugi::xml_document programDocument;
		pugi::xml_parse_result parseResult = programDocument.load_file(file.c_str(), pugi::parse_default, pugi::xml_encoding::encoding_utf8);
		if (!parseResult) {
			throw std::runtime_error("Failed to parse document at file: " + file + ". Error: " + std::string(parseResult.description()));
		}
		return ParseProgramConfig(programDocument);
	}

	void ShaderProgramConfig::AddShaderConfig(const ShaderConfig& shaderData) {
		m_shaderConfigs.push_back(shaderData);
	}

	void ShaderProgramConfig::SetName(const std::string& name) {
		m_name = name;
	}

	const std::string& ShaderProgramConfig::GetName() const {
		return m_name;
	}

	const std::vector<ShaderConfig>& ShaderProgramConfig::getShaderConfigs() const {
		return m_shaderConfigs;
	}
}