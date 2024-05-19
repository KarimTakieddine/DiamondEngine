#pragma once

#include "AnimationPlayer.h"
#include "SpriteSheet.h"

namespace diamond_engine
{
	struct SpriteSheetAnimation
	{
		SpriteSheet spriteSheet;
		Animation animation;
	};

	class MaterialRenderComponent;
	class SpriteAnimationPlayer : public AnimationPlayer
	{
	public:
		virtual ~SpriteAnimationPlayer() final override = default;

		void setAnimations(const std::vector<Animation>& animations);
		void playAnimation(const std::string& name, bool immediate = false);

		void animate(const Animation& animation, GLfloat deltaTime) final override;
		EngineStatus initialize(const BehaviourComponentConfig* config) final override;

		const char* getName() const final override;

	private:
		std::unordered_map<std::string, SpriteSheetAnimation> m_spriteSheetAnimations;
		MaterialRenderComponent* m_material{ nullptr };
		const SpriteSheet* m_currentSpriteSheet{ nullptr };
	};
}
