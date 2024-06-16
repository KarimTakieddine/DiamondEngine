#pragma once

#include <vector>

#include <GL/glew.h>

#include "SpriteAnimation.h"
#include "BehaviourComponentConfig.h"

namespace diamond_engine {
	class SpriteAnimationPlayerConfig : public BehaviourComponentConfig {
	public:
		virtual ~SpriteAnimationPlayerConfig() final override = default;

		const std::vector<SpriteAnimation>& getAnimations() const;

		void addAnimation(const SpriteAnimation& animation);

		const char* getName() const final override {
			return "SpriteAnimationPlayer";
		}

	private:
		std::vector<SpriteAnimation> m_animations;
	};
}
