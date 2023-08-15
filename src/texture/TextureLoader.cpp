#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureMetadataParser.h"
#include "TextureLoader.h"

namespace diamond_engine {
	/* static */ const std::string TextureLoader::kTextureCollectionMetadataFilename = "textureMetadata.xml";

	TextureLoader::TextureLoader() : m_textureAllocator(std::make_unique<GLAllocator>(glGenTextures, glDeleteTextures)) { }

	void TextureLoader::Load(const std::string& rootDirectory) {
		const std::filesystem::path rootDirectoryPath(rootDirectory);

		if (!std::filesystem::is_directory(rootDirectoryPath)) {
			throw std::runtime_error("Root texture directory: " + rootDirectory + " is not a directory");
		}

		const std::filesystem::path textureMetadataPath = rootDirectoryPath / kTextureCollectionMetadataFilename;

		if (!std::filesystem::exists(textureMetadataPath) || std::filesystem::is_directory(textureMetadataPath)) {
			throw std::runtime_error("Texture metadata file: " + textureMetadataPath.string() + " does not exist");
		}

		const TextureCollectionMetadata textureCollectionMetadata = TextureMetadataParser::ParseCollection(textureMetadataPath.string());

		const auto& textureMetadataList = textureCollectionMetadata.GetTextures();

		const size_t maxTextureCount = textureMetadataList.size();

		m_textureAllocator->Reserve(maxTextureCount);
		m_textureMap.reserve(maxTextureCount);
		for (const auto& textureMetadata : textureMetadataList) {
			Texture texture;
			
			std::filesystem::path texturePath(textureMetadata.GetPath());
			if (texturePath.is_relative()) {
				texturePath = rootDirectoryPath / texturePath;
			}
			loadTexture(texturePath, texture);

			texture.index		= m_textureAllocator->Get();
			texture.wrapModeS	= textureMetadata.GetWrapModeS();
			texture.wrapModeT	= textureMetadata.GetWrapModeT();

			AddTexture(textureMetadata.GetName(), texture);
		}
	}

	void TextureLoader::AddTexture(const std::string& name, const Texture& texture) {
		m_textureMap.emplace(name, texture);
	}
	
	void TextureLoader::DeleteTexture(const std::string& name) {
		auto textureIt = m_textureMap.find(name);

		if (textureIt == m_textureMap.end()) {
			return;
		}

		m_textureMap.erase(textureIt);
	}

	Texture TextureLoader::GetTexture(const std::string& name) const {
		auto textureIt = m_textureMap.find(name);

		return textureIt == m_textureMap.end() ? Texture() : textureIt->second;
	}
	
	GLint TextureLoader::GetTextureIndex(const std::string& name) const {
		return GetTexture(name).index;
	}

	void TextureLoader::loadTexture(const std::filesystem::path& texturePath, Texture& texture) {
		// TODO!
	}
}