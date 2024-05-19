#pragma once

#include <vector>

#include <GL/glew.h>

#include "Animation.h"
#include "BehaviourComponentConfig.h"

namespace diamond_engine {
	class SpriteAnimationPlayerConfig : public BehaviourComponentConfig {
	public:
		virtual ~SpriteAnimationPlayerConfig() final override = default;

		const std::vector<Animation>& getAnimations() const;

		void addAnimation(const Animation& animation);

		const char* getName() const final override {
			return "SpriteAnimationPlayer";
		}

	private:
		std::vector<Animation> m_animations;
	};
}
