#pragma once

#include <vector>

#include <GL/glew.h>

#include "Animation.h"
#include "ComponentConfig.h"

namespace diamond_engine {
	class SpriteAnimationPlayerConfig : public ComponentConfig {
	public:
		virtual ~SpriteAnimationPlayerConfig() override = default;

		const std::vector<Animation>& getAnimations() const;

		void addAnimation(const Animation& animation);

		const char* GetName() const override {
			return "SpriteAnimationPlayer";
		}

	private:
		std::vector<Animation> m_animations;
	};
}
