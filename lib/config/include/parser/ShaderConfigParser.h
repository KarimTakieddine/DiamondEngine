#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

namespace pugi { class xml_document; class xml_node; }

namespace diamond_engine {
struct ShaderConfig {
	std::string file;
	GLenum type{ GL_VERTEX_SHADER };

	std::vector<std::string> attributes;
	std::vector<std::string> uniforms;
};

class ShaderProgramConfig {
public:
	void AddShaderConfig(const ShaderConfig& shaderConfig);

	void SetName(const std::string& name);

	const std::string& GetName() const;

	const std::vector<ShaderConfig>& getShaderConfigs() const;

private:
	std::vector<ShaderConfig> m_shaderConfigs{ };
	std::string m_name{ };
};

class ShaderConfigParser {
public:
	using StringToShaderTypeMap = std::unordered_map<std::string, GLenum>;

	static const StringToShaderTypeMap kStringToShaderTypeMap;

	static GLenum ShaderTypeFromString(const std::string& shaderType);

	static ShaderConfig ParseShaderConfig(const pugi::xml_node& shaderNode);

	static ShaderProgramConfig ParseProgramConfig(const pugi::xml_document& programDocument);

	static ShaderProgramConfig ParseProgramConfig(const std::string& file);
};
}
