#include <deque>
#include <stdexcept>

#include "File.h"
#include "SharedShaderStore.h"
#include "ShaderMetadataParser.h"

namespace diamond_engine {
	/* static */ const std::string SharedShaderStore::kProgramMetadataFilename	= "programMetadata.xml";

	void SharedShaderStore::Load(const std::string& rootDirectory) {
		const auto programDirectories = GetShaderDirectories(rootDirectory);

		for (const auto& programDirectory : programDirectories) {
			const std::filesystem::path programMetadataPath = programDirectory / kProgramMetadataFilename;

			if (!std::filesystem::exists(programMetadataPath) || !std::filesystem::is_regular_file(programMetadataPath)) {
				throw std::runtime_error("Could not find programMetadata.xml file in program folder: " + programDirectory.string());
			}

			const ShaderProgramMetadata programMetadata = ShaderMetadataParser::LoadShaderProgramMetadata(programMetadataPath.string());


		}
	}

	const std::shared_ptr<Shader> SharedShaderStore::FindShader(const std::string& file) {
		/*auto shader = m_store.find(file);

		if (shader == m_store.end()) {
			throw std::runtime_error("Unable to find shader with file key: " + file);
		}

		return m_store.at(file);*/

		return nullptr;
	}

	std::vector<std::filesystem::path> SharedShaderStore::GetShaderDirectories(const std::string& rootDirectory) {
		std::vector<std::filesystem::path> result;
		
		for (const auto& childPath : std::filesystem::directory_iterator(rootDirectory)) {
			if (std::filesystem::is_directory(childPath)) {
				result.push_back(childPath);
			}
		}

		return result;
	}
}