#include "SpriteAnimationPlay.h"

namespace diamond_engine
{
	SpriteAnimationPlay::SpriteAnimationPlay(const std::string& name, bool immediate) : m_name(name), m_immediate(immediate) { }

	const std::string& SpriteAnimationPlay::getName() const
	{
		return m_name;
	}

	void SpriteAnimationPlay::setName(const std::string& name)
	{
		m_name = name;
	}

	bool SpriteAnimationPlay::immediate() const
	{
		return m_immediate;
	}

	void SpriteAnimationPlay::setImmediate(bool immediate)
	{
		m_immediate = immediate;
	}
}