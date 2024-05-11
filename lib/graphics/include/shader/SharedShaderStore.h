#pragma once

#include <filesystem>
#include <unordered_map>

#include "ShaderProgram.h"

namespace diamond_engine {
class SharedShaderStore {
public:
	static const std::string kProgramMetadataFilename;

	void Load(const std::string& rootDirectory);

	void unload();

	std::shared_ptr<ShaderProgram> FindProgram(const std::string& name) const;

	static std::shared_ptr<SharedShaderStore>& getInstance();

private:
	SharedShaderStore() = default;

	static std::vector<std::filesystem::path> GetProgramDirectories(const std::string& rootDirectory);

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_store;
};
}
