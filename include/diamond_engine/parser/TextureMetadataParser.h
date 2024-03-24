#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <pugixml.hpp>

namespace diamond_engine {
class TextureMetadata {
public:
	void SetName(const std::string& name);

	const std::string& GetName() const;

	void SetWrapModeS(GLenum wrapModeS);

	GLenum GetWrapModeS() const;

	void SetWrapModeT(GLenum wrapModeT);

	GLenum GetWrapModeT() const;

	void SetMinFilter(GLenum minFilter);

	GLenum GetMinFilter() const;

	void SetMagFilter(GLenum magFilter);

	GLenum GetMagFilter() const;

	void SetPath(const std::string& path);

	const std::string& GetPath() const;

private:
	std::string m_name;
	std::string m_path;
	GLenum m_wrapModeS{ GL_REPEAT };
	GLenum m_wrapModeT{ GL_REPEAT };
	GLenum m_minFilter{ GL_NEAREST };
	GLenum m_magFilter{ GL_NEAREST };
};

class TextureCollectionMetadata {
public:
	const std::vector<TextureMetadata>& GetTextures() const;

	void AddTexture(const TextureMetadata& texture);

	void setMaxTextureCount(GLuint maxTextureCount);
	GLuint getMaxTextureCount() const;

private:
	std::vector<TextureMetadata> m_textures;
	GLuint m_maxTextureCount{ 256 };
};

class TextureMetadataParser {
public:
	static const std::unordered_map<std::string, GLenum> kStringToWrapModeMap;

	static const std::unordered_map<std::string, GLenum> kStringToFilterMap;

	static TextureMetadata Parse(const pugi::xml_node& textureNode);

	static TextureCollectionMetadata ParseCollection(const pugi::xml_document& textureCollectionDocument);

	static TextureCollectionMetadata ParseCollection(const std::string& file);

private:
	static GLenum parseWrapMode(const pugi::xml_node& textureNode, const std::string& attributeName);

	static GLenum parseFilter(const pugi::xml_node& textureNode, const std::string& attributeName);
};
}
