#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include <pugixml.hpp>

namespace diamond_engine {
struct ShaderMetadata {
	std::string file;
	GLenum type{ GL_VERTEX_SHADER };

	std::vector<std::string> attributes;
	std::vector<std::string> uniforms;
};

class ShaderProgramMetadata {
public:
	void AddShaderData(const ShaderMetadata& shaderData);

	void SetName(const std::string& name);

	const std::string& GetName() const;

	const std::vector<ShaderMetadata>& getShaderEntries() const;

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
