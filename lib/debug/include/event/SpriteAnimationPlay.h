#pragma once

#include <string>

#include "DebugEvent.h"

namespace diamond_engine
{
	class SpriteAnimationPlay : public DebugEvent
	{
	public:
		~SpriteAnimationPlay() final override = default;

		SpriteAnimationPlay(const std::string& name, bool immediate);

		const std::string& getName() const;
		void setName(const std::string& name);

		bool immediate() const;
		void setImmediate(bool immediate);

	private:
		std::string m_name{ "Unnamed" };
		bool m_immediate{ false };
	};
}
