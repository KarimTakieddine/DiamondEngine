#include <deque>
#include <stdexcept>

#include "File.h"
#include "SharedShaderStore.h"

namespace diamond_engine {
	/* static */ const std::string SharedShaderStore::kVertexShaderPrefix	= "vertex";
	/* static */ const std::string SharedShaderStore::kFragmentShaderPrefix = "fragment";

	void SharedShaderStore::Load(const std::string& rootDirectory) {
		const auto shaderPaths = GetShaderFiles(rootDirectory);

		for (const auto& path : shaderPaths) {
			const std::string fileName = path.filename().string();

			auto dotPosition = fileName.find('.', 0);

			if (dotPosition == std::string::npos) {
				dotPosition = fileName.size() - 1;
			}

			const std::string fileNameNoDot = fileName.substr(0, dotPosition);

			auto underscorePosition = fileNameNoDot.find('_', 0);

			if (underscorePosition == std::string::npos) {
				underscorePosition = fileNameNoDot.size() - 1;
			}

			const std::string prefix = fileNameNoDot.substr(0, underscorePosition);

			std::shared_ptr<Shader> shader{ nullptr };
			if (prefix == kVertexShaderPrefix) {
				shader = std::make_shared<Shader>(GL_VERTEX_SHADER);
			}
			else if (prefix == kFragmentShaderPrefix) {
				shader = std::make_shared<Shader>(GL_FRAGMENT_SHADER);
			}

			const std::string& file = path.string();
			
			if (!shader->Load(file)) {
				throw std::runtime_error("Shader compilation failed - File: " + fileName);
			}

			m_store.insert({ file, shader });
		}
	}

	std::vector<std::filesystem::path> SharedShaderStore::GetShaderFiles(const std::string& rootDirectory) {
		std::vector<std::filesystem::path> result;
		
		const std::filesystem::path rootDirectoryPath(rootDirectory);
		
		std::deque<std::filesystem::path> parentPaths;
		parentPaths.push_back(rootDirectoryPath);

		while (!parentPaths.empty()) {
			const std::filesystem::path parentPath = parentPaths.front();

			for (const auto& childPath : std::filesystem::directory_iterator(parentPath)) {
				if (std::filesystem::is_directory(childPath)) {
					parentPaths.push_back(childPath);
				}
				else if (std::filesystem::is_regular_file(childPath)) {
					result.push_back(childPath);
				}
			}

			parentPaths.pop_front();
		}

		return result;
	}
}