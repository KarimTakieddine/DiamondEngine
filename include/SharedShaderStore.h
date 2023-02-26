#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace diamond_engine {
class Shader;

class SharedShaderStore {
public:
	static const std::string kVertexShaderPrefix;
	static const std::string kFragmentShaderPrefix;

	void Load(const std::string& rootDirectory);

private:
	static std::vector<std::filesystem::path> GetShaderFiles(const std::string& rootDirectory);

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_store;
};
}
