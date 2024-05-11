#include <pugixml.hpp>

#include <stdexcept>

#include "TextureMetadataParser.h"

namespace diamond_engine {
	void TextureMetadata::SetName(const std::string& name) {
		m_name = name;
	}

	const std::string& TextureMetadata::GetName() const {
		return m_name;
	}

	void TextureMetadata::SetWrapModeS(GLenum wrapModeS) {
		m_wrapModeS = wrapModeS;
	}

	GLenum TextureMetadata::GetWrapModeS() const {
		return m_wrapModeS;
	}

	void TextureMetadata::SetWrapModeT(GLenum wrapModeT) {
		m_wrapModeT = wrapModeT;
	}

	GLenum TextureMetadata::GetWrapModeT() const {
		return m_wrapModeT;
	}

	void TextureMetadata::SetMinFilter(GLenum minFilter) {
		m_minFilter = minFilter;
	}

	GLenum TextureMetadata::GetMinFilter() const {
		return m_minFilter;
	}

	void TextureMetadata::SetMagFilter(GLenum magFilter) {
		m_magFilter = magFilter;
	}

	GLenum TextureMetadata::GetMagFilter() const {
		return m_magFilter;
	}

	void TextureMetadata::SetPath(const std::string& path) {
		m_path = path;
	}

	const std::string& TextureMetadata::GetPath() const {
		return m_path;
	}

	const std::vector<TextureMetadata>& TextureCollectionMetadata::GetTextures() const {
		return m_textures;
	}

	void TextureCollectionMetadata::AddTexture(const TextureMetadata& texture) {
		m_textures.push_back(texture);
	}

	GLuint TextureCollectionMetadata::getMaxTextureCount() const {
		return m_maxTextureCount;
	}

	void TextureCollectionMetadata::setMaxTextureCount(GLuint maxTextureCount) {
		m_maxTextureCount = maxTextureCount;
	}

	/* static */ const std::unordered_map<std::string, GLenum> TextureMetadataParser::kStringToWrapModeMap = {
		{ "repeat",					GL_REPEAT },
		{ "mirroredRepeat",			GL_MIRRORED_REPEAT },
		{ "clampToBorder",			GL_CLAMP_TO_BORDER },
		{ "clampToEdge",			GL_CLAMP_TO_EDGE },
		{ "mirroredClampToEdge",	GL_MIRROR_CLAMP_TO_EDGE }
	};

	/* static */ const std::unordered_map<std::string, GLenum> TextureMetadataParser::kStringToFilterMap = {
		{ "nearest", GL_NEAREST },
		{ "linear", GL_LINEAR }
	};

	/* static */ TextureMetadata TextureMetadataParser::Parse(const pugi::xml_node& textureNode) {
		TextureMetadata result;

		pugi::xml_attribute nameAttribute = textureNode.attribute("name");
		if (!nameAttribute) {
			throw std::runtime_error("Could not find \"name\" attribute on <Texture/> node");
		}
		result.SetName(nameAttribute.as_string());

		pugi::xml_attribute pathAttribute = textureNode.attribute("path");
		if (!pathAttribute) {
			throw std::runtime_error("Could not find \"path\" attribute on <Texture/> node");
		}
		result.SetPath(pathAttribute.as_string());

		result.SetWrapModeS(parseWrapMode(textureNode, "wrapModeS"));
		result.SetWrapModeT(parseWrapMode(textureNode, "wrapModeT"));

		return result;
	}

	/* static */ TextureCollectionMetadata TextureMetadataParser::ParseCollection(const pugi::xml_document& textureCollectionDocument) {
		pugi::xml_node textureCollectionNode = textureCollectionDocument.root().first_child();

		if (!textureCollectionNode) {
			throw std::runtime_error("No root node specified for texture collection document");
		}

		const std::string textureCollectionNodeName(textureCollectionNode.name());

		if (textureCollectionNodeName != "TextureCollection") {
			throw std::runtime_error("Invalid root node name: " + textureCollectionNodeName + " specified for <TextureCollection/> document");
		}

		TextureCollectionMetadata result;

		pugi::xml_attribute maxTextureCountAttribute = textureCollectionNode.attribute("maxTextureCount");
		if (maxTextureCountAttribute)
		{
			result.setMaxTextureCount(maxTextureCountAttribute.as_uint(result.getMaxTextureCount()));
		}

		for (const auto& textureNode : textureCollectionNode.children("Texture")) {
			result.AddTexture(Parse(textureNode));
		}
		return result;
	}

	/* static */ TextureCollectionMetadata TextureMetadataParser::ParseCollection(const std::string& file) {
		pugi::xml_document textureCollectionDocument;

		pugi::xml_parse_result parseResult = textureCollectionDocument.load_file(file.c_str(), pugi::parse_default, pugi::xml_encoding::encoding_utf8);
		if (!parseResult) {
			throw std::runtime_error("Failed to parse <TextureColletion/> document from file: " + file + ". Error was: " + std::string(parseResult.description()));
		}

		return ParseCollection(textureCollectionDocument);
	}

	/* static */ GLenum TextureMetadataParser::parseWrapMode(const pugi::xml_node & textureNode, const std::string & attributeName) {
		pugi::xml_attribute wrapModeAttribute = textureNode.attribute(attributeName.c_str());

		if (!wrapModeAttribute) {
			return GL_REPEAT; // Default
		}

		const std::string wrapModeString(wrapModeAttribute.as_string());

		auto wrapModeIt = kStringToWrapModeMap.find(wrapModeString);

		if (wrapModeIt == kStringToWrapModeMap.end()) {
			throw std::runtime_error("Invalid wrap mode specified: " + wrapModeString);
		}

		return wrapModeIt->second;
	}

	/* static */ GLenum TextureMetadataParser::parseFilter(const pugi::xml_node& textureNode, const std::string& attributeName) {
		pugi::xml_attribute filterAttribute = textureNode.attribute(attributeName.c_str());

		if (!filterAttribute) {
			return GL_NEAREST; // Default
		}

		const std::string filterString(filterAttribute.as_string());

		auto filterIt = kStringToFilterMap.find(filterString);

		if (filterIt == kStringToFilterMap.end()) {
			throw std::runtime_error("Invalid filter type specified: " + filterString);
		}

		return filterIt->second;
	}
}