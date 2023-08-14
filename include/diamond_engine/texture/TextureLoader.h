#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "GLAllocator.h"
#include "Texture.h"

namespace diamond_engine {
class TextureLoader {
public:
	using TextureMap = std::unordered_map<std::string, Texture>;

	static const std::string kTextureCollectionMetadataFilename;

	TextureLoader();

	void Load(const std::string& rootDirectory);

	void AddTexture(const std::string& name, const Texture& texture);

	void DeleteTexture(const std::string& name);

	const Texture& GetTexture(const std::string& name) const;

	GLint GetTextureIndex(const std::string& name) const;

private:
	void loadTexture(const std::filesystem::path& texturePath, Texture& texture);

	TextureMap m_textureMap;
	std::unique_ptr<GLAllocator> m_textureAllocator{ nullptr };
};
}
