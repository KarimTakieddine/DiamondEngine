#include <deque>
#include <fstream>
#include <stdexcept>

#include "SharedShaderStore.h"
#include "ShaderConfigParser.h"

namespace
{
	static std::string readFile(const std::string& path) {
		std::ifstream inputFileStream(path, std::ifstream::binary);

		if (!inputFileStream.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}

		inputFileStream.seekg(0, inputFileStream.end);
		auto fileSize = inputFileStream.tellg();
		inputFileStream.seekg(0, inputFileStream.beg);

		std::vector<char> contentsBuffer(
			static_cast<size_t>(fileSize) + 1,
			'\0');

		inputFileStream.read(contentsBuffer.data(), fileSize);
		inputFileStream.close();

		return std::string(contentsBuffer.data());
	}
}

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

			const ShaderProgramConfig programConfig = ShaderConfigParser::ParseProgramConfig(programMetadataPath.string());

			std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();

			const auto& shaderConfigs = programConfig.getShaderConfigs();

			for (const auto& shaderConfig : shaderConfigs) {
				const std::filesystem::path sourcePath(shaderConfig.file);

				std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderConfig.type);
				shader->SetSource(
					readFile(
						sourcePath.is_absolute() ?
						sourcePath.string() :
						(programDirectory / sourcePath).string()));
				shader->Compile();

				if (!shader->IsCompiled()) {
					throw std::runtime_error("Failed to compile shader defined in file: " + shaderConfig.file);
				}

				shaderProgram->AttachShader(shader);
			}
			shaderProgram->Link();

			if (!shaderProgram->IsLinked()) {
				throw std::runtime_error("Failed to link program named: " + programConfig.GetName());
			}

			for (const auto& shaderConfig : shaderConfigs) {
				// TODO: Clearing logic somewhere?
				shaderProgram->LoadAttributes(shaderConfig.attributes);
				shaderProgram->LoadUniforms(shaderConfig.uniforms);
			}

			m_store.emplace(programConfig.GetName(), shaderProgram);
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