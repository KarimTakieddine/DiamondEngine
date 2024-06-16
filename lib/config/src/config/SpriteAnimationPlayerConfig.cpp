#include "SpriteAnimationPlayerConfig.h"

namespace diamond_engine
{
	const std::vector<SpriteAnimation>& SpriteAnimationPlayerConfig::getAnimations() const
	{
		return m_animations;
	}

	void SpriteAnimationPlayerConfig::addAnimation(const SpriteAnimation& animation)
	{
		m_animations.push_back(animation);
	}
}