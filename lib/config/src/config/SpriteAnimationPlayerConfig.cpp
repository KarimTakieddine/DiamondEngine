#include "SpriteAnimationPlayerConfig.h"

namespace diamond_engine
{
	const std::vector<Animation>& SpriteAnimationPlayerConfig::getAnimations() const
	{
		return m_animations;
	}

	void SpriteAnimationPlayerConfig::addAnimation(const Animation& animation)
	{
		m_animations.push_back(animation);
	}
}