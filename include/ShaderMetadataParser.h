#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include <pugixml.hpp>

namespace diamond_engine {
struct ShaderMetadata {
	std::string file;
	GLenum type;
};

class ShaderProgramMetadata {
public:
	void AddShaderData(const ShaderMetadata& shaderData);

	void SetName(const std::string& name);

private:
	std::vector<ShaderMetadata> m_shaderData{ };
	std::string m_name{ };
};

class ShaderMetadataParser {
public:
	using StringToShaderTypeMap = std::unordered_map<std::string, GLenum>;

	static const StringToShaderTypeMap kStringToShaderTypeMap;

	static GLenum ShaderTypeFromString(const std::string& shaderType);

	static ShaderMetadata ParseShaderMetadata(const pugi::xml_node& shaderNode);

	static ShaderProgramMetadata ParseShaderProgramMetadata(const pugi::xml_document& programDocument);

	static ShaderProgramMetadata LoadShaderProgramMetadata(const std::string& file);
};
}
