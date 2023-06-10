#pragma once

#include <filesystem>
#include <unordered_map>

#include "ShaderProgram.h"

namespace diamond_engine {
class SharedShaderStore {
public:
	static const std::string kProgramMetadataFilename;

	void Load(const std::string& rootDirectory);

	const std::shared_ptr<Shader> FindShader(const std::string& file);

private:
	static std::vector<std::filesystem::path> GetShaderDirectories(const std::string& rootDirectory);

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_store;
};
}
