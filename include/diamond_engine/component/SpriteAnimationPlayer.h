#pragma once

#include "AnimationPlayer.h"
#include "SpriteSheetLoader.h"

namespace diamond_engine
{
	struct SpriteSheetAnimation
	{
		SpriteSheet spriteSheet;
		Animation animation;
	};

	class SpriteAnimationPlayer : public AnimationPlayer
	{
	public:
		void setSharedSpriteSheetLoader(const std::shared_ptr<SpriteSheetLoader>& spriteSheetLoader);
		void setAnimations(const std::vector<Animation>& animations);
		void playAnimation(const std::string& name, bool immediate = false);

		void animate(const Animation& animation, GLfloat deltaTime) final override;

		void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;
		void OnAddedToScene() final override;
		const char* GetName() const final override;

		virtual ~SpriteAnimationPlayer() final override = default;

	private:
		std::unordered_map<std::string, SpriteSheetAnimation> m_spriteSheetAnimations;
		std::shared_ptr<SpriteSheetLoader> m_sharedSpriteSheetLoader{ nullptr };
		std::vector<Animation> m_animations;
		const SpriteSheet* m_currentSpriteSheet{ nullptr };
	};
}
