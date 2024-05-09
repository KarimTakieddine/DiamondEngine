#include <deque>
#include <stdexcept>

#include "File.h"
#include "SharedShaderStore.h"
#include "ShaderMetadataParser.h"

namespace diamond_engine {
	/* static */ const std::string SharedShaderStore::kProgramMetadataFilename	= "programMetadata.xml";

	/* static */ std::shared_ptr<SharedShaderStore>& SharedShaderStore::getInstance()
	{
		static std::shared_ptr<SharedShaderStore> sharedShaderStore(new SharedShaderStore());
		return sharedShaderStore;
	}

	void SharedShaderStore::Load(const std::string& rootDirectory) {
		unload();

		const auto programDirectories = GetProgramDirectories(rootDirectory);

		m_store.reserve(programDirectories.size());
		for (const auto& programDirectory : programDirectories) {
			const std::filesystem::path programMetadataPath = programDirectory / kProgramMetadataFilename;

			if (!std::filesystem::exists(programMetadataPath) || !std::filesystem::is_regular_file(programMetadataPath)) {
				continue;
			}

			const ShaderProgramMetadata programMetadata = ShaderMetadataParser::LoadShaderProgramMetadata(programMetadataPath.string());

			std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();

			const auto& shaderMetadataEntries = programMetadata.getShaderEntries();

			for (const auto& shaderMetadata : shaderMetadataEntries) {
				const std::filesystem::path sourcePath(shaderMetadata.file);

				std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderMetadata.type);
				shader->SetSource(
					File::Read(
						sourcePath.is_absolute() ?
						sourcePath.string() :
						(programDirectory / sourcePath).string()));
				shader->Compile();

				if (!shader->IsCompiled()) {
					throw std::runtime_error("Failed to compile shader defined in file: " + shaderMetadata.file);
				}

				shaderProgram->AttachShader(shader);
			}
			shaderProgram->Link();

			if (!shaderProgram->IsLinked()) {
				throw std::runtime_error("Failed to link program named: " + programMetadata.GetName());
			}

			for (const auto& shaderMetadata : shaderMetadataEntries) {
				// TODO: Clearing logic somewhere?
				shaderProgram->LoadAttributes(shaderMetadata.attributes);
				shaderProgram->LoadUniforms(shaderMetadata.uniforms);
			}

			m_store.emplace(programMetadata.GetName(), shaderProgram);
		}
	}

	void SharedShaderStore::unload()
	{
		m_store.clear();
	}

	std::shared_ptr<ShaderProgram> SharedShaderStore::FindProgram(const std::string& name) const {
		const auto it = m_store.find(name);
		return it == m_store.cend() ? nullptr : it->second;
	}

	std::vector<std::filesystem::path> SharedShaderStore::GetProgramDirectories(const std::string& rootDirectory) {
		const std::filesystem::path rootDirectoryPath(rootDirectory);

		if (!std::filesystem::exists(rootDirectoryPath) || !std::filesystem::is_directory(rootDirectoryPath)) {
			throw std::runtime_error("Provided root directory path: " + rootDirectory + " is not an existing directory");
		}

		std::vector<std::filesystem::path> result;
		
		for (const auto& childPath : std::filesystem::directory_iterator(rootDirectory)) {
			if (std::filesystem::is_directory(childPath)) {
				result.push_back(childPath);
			}
		}

		return result;
	}
}