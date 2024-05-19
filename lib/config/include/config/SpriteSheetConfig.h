#pragma once

#include <string>
#include <vector>

#include "FrameConfig.h"

namespace diamond_engine
{
	class SpriteSheetConfig
	{
	public:
		void addFrame(const FrameConfig& frame)
		{
			m_frames.push_back(frame);
		}

		const std::vector<FrameConfig>& getFrames() const
		{
			return m_frames;
		}

		void setName(const std::string& name)
		{
			m_name = name;
		}

		const std::string& getName() const
		{
			return m_name;
		}

		void setSpriteSheet(const std::string& spriteSheet)
		{
			m_spriteSheet = spriteSheet;
		}

		const std::string& getSpriteSheet() const
		{
			return m_spriteSheet;
		}

	private:
		std::vector<FrameConfig> m_frames;
		std::string m_name{ "Unnamed" };
		std::string m_spriteSheet{ "Unnamed" };
	};
}
