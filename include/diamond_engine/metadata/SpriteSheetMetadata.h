#pragma once

#include <string>
#include <vector>

#include "FrameMetadata.h"

namespace diamond_engine
{
	class SpriteSheetMetadata
	{
	public:
		void addFrame(const FrameMetadata& frame)
		{
			m_frames.push_back(frame);
		}

		const std::vector<FrameMetadata>& getFrames() const
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

	private:
		std::vector<FrameMetadata> m_frames;
		std::string m_name{ "Unnamed" };
	};
}
