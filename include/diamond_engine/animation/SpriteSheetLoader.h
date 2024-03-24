#pragma once

#include <vector>

#include "SpriteSheetMetadataParser.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	struct SpriteSheet
	{
		std::vector<Texture> frames;
	};

	class SpriteSheetLoader
	{
	public:
		static const std::string kMetadataFilename;

		void setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader);
		void loadSpriteSheets(const SpriteSheetList& spriteSheets);
		void load(const std::string& directory);
		const SpriteSheet& getSpriteSheet(const std::string& name) const;

	private:
		std::unordered_map<std::string, SpriteSheet> m_spriteSheets;
		std::shared_ptr<TextureLoader> m_sharedTextureLoader{ nullptr };
	};
}
