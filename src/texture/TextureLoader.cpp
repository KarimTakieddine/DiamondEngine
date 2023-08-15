#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureMetadataParser.h"
#include "TextureLoader.h"

namespace diamond_engine {
	/* static */ const std::string TextureLoader::kTextureCollectionMetadataFilename = "textureMetadata.xml";

	/* static */ const std::unordered_map<GLint, GLenum> TextureLoader::kChannelCountToFormatMap = {
		{ 3, GL_RGB },
		{ 4, GL_RGBA }
	};

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
		m_textureAllocator->Allocate(maxTextureCount);
		m_textureMap.reserve(maxTextureCount);
		for (const auto& textureMetadata : textureMetadataList) {
			Texture texture;
			
			std::filesystem::path texturePath(textureMetadata.GetPath());
			if (texturePath.is_relative()) {
				texturePath = rootDirectoryPath / texturePath;
			}

			texture.index		= m_textureAllocator->Get();
			texture.wrapModeS	= textureMetadata.GetWrapModeS();
			texture.wrapModeT	= textureMetadata.GetWrapModeT();
			texture.minFilter	= textureMetadata.GetMinFilter();
			texture.magFilter	= textureMetadata.GetMagFilter();

			loadTexture(texturePath, texture);

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
		GLint width			= 0;
		GLint height		= 0;
		GLint channelCount	= 0;

		GLubyte* imageData = stbi_load(texturePath.string().c_str(), &width, &height, &channelCount, 0);

		if (!imageData) {
			throw std::runtime_error("Failed to load image from path: " + texturePath.string());
		}

		auto formatIt = kChannelCountToFormatMap.find(channelCount);
		if (formatIt == kChannelCountToFormatMap.end()) {
			throw std::runtime_error("Could not find format matching channel count: " + std::to_string(channelCount));
		}

		glBindTexture(GL_TEXTURE_2D, texture.index);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.wrapModeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.wrapModeT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture.magFilter);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, formatIt->second, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
	}
}