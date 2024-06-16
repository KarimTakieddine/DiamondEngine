#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SpriteAnimation.h"
#include "SpriteSheetConfigParser.h"

namespace diamond_engine
{
	// TODO: Rename to AnimationEngine or something similar

	class SpriteSheetLoader
	{
	public:
		static const std::string kMetadataFilename;

		static std::shared_ptr<SpriteSheetLoader>& getInstance();

		void loadSpriteSheets(const SpriteSheetConfigList& spriteSheets);
		void load(const std::string& directory);
		const SpriteAnimation& getSpriteSheet(const std::string& name) const;

	private:
		SpriteSheetLoader() = default;

		std::unordered_map<std::string, SpriteAnimation> m_spriteSheets;
	};
}
