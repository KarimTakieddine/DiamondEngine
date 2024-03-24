#pragma once

#include "AnimationPlayer.h"
#include "Component.h"
#include "Texture.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	struct SpriteSheet
	{
		std::vector<Texture> m_frames;
	};

	struct SpriteSheetAnimation
	{
		SpriteSheet spriteSheet;
		Animation animation;
	};

	class SpriteAnimationPlayer : public AnimationPlayer
	{
	public:
		void setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader);

		virtual void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) override;
		virtual void OnAddedToScene() override;
		virtual const char* GetName() const override;

		virtual ~SpriteAnimationPlayer() override = default;

	private:
		void loadSpriteSheet(const std::string& spriteSheetName);

		std::unordered_map<std::string, SpriteSheetAnimation> m_spriteSheetAnimations;
		std::vector<std::string> m_spriteSheetNames;
		std::shared_ptr<TextureLoader> m_sharedTextureLoader{ nullptr };
	};
}
