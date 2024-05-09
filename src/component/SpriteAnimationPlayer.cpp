#include <stdexcept>

#include "SpriteAnimationPlayer.h"

#include <iostream>

namespace diamond_engine
{
	void SpriteAnimationPlayer::setSharedSpriteSheetLoader(const std::shared_ptr<SpriteSheetLoader>& spriteSheetLoader)
	{
		m_sharedSpriteSheetLoader = spriteSheetLoader;
	}

	void SpriteAnimationPlayer::setAnimations(const std::vector<Animation>& animations)
	{
		m_animations = animations;
	}

	void SpriteAnimationPlayer::playAnimation(const std::string& name, bool immediate /* = false */)
	{
		auto it = m_spriteSheetAnimations.find(name);

		if (it == m_spriteSheetAnimations.cend())
		{
			throw std::runtime_error("Could not find sprite animation named: " + name);
		}

		SpriteSheetAnimation& spriteSheetAnimation = it->second;

		m_currentSpriteSheet = &spriteSheetAnimation.spriteSheet;
		Animation& animation = spriteSheetAnimation.animation;

		animation.frame = 0;

		play(animation, immediate);
	}

	void SpriteAnimationPlayer::animate(const Animation& animation, GLfloat deltaTime)
	{
		if (!m_currentSpriteSheet)
		{
			return;
		}

		const GLuint animationFrame = animation.frame;

		if (animationFrame >= m_currentSpriteSheet->frames.size())
		{
			return;
		}

		//m_gameObject->GetRenderableObject()->material.SetTexture(m_currentSpriteSheet->frames[animationFrame].index);
	}

	void SpriteAnimationPlayer::BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) { }

	void SpriteAnimationPlayer::OnAddedToScene()
	{
		if (!m_sharedSpriteSheetLoader)
		{
			throw std::runtime_error("No sprite sheet loader was set to SpriteAnimationPlayer instance");
		}

		m_spriteSheetAnimations.clear();
		m_currentSpriteSheet = nullptr;
		for (auto& animation : m_animations)
		{
			const std::string& spriteSheetName = animation.name;
			const auto& spriteSheet = m_sharedSpriteSheetLoader->getSpriteSheet(spriteSheetName);

			animation.timeBetweenFrames = ( animation.duration / spriteSheet.frames.size() ) + animation.delayBetweenFrames;

			m_spriteSheetAnimations.insert({ spriteSheetName, { spriteSheet, animation  } });
		}
	}

	const char* SpriteAnimationPlayer::GetName() const
	{
		return "AnimationPlayer";
	}
}