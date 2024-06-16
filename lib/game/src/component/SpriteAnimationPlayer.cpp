#include <stdexcept>

#include "GameInstance.h"
#include "MaterialRenderComponent.h"
#include "SpriteAnimationPlayer.h"
#include "SpriteAnimationPlayerConfig.h"
#include "SpriteSheetLoader.h"

namespace diamond_engine
{
	void SpriteAnimationPlayer::playAnimation(const std::string& name, bool immediate /* = false */)
	{
		auto it = m_spriteAnimations.find(name);

		if (it == m_spriteAnimations.cend())
			throw std::runtime_error("Could not find sprite animation named: " + name);

		SpriteAnimation& spriteAnimation = it->second;

		play(spriteAnimation, immediate);
	}

	void SpriteAnimationPlayer::animate(const SpriteAnimation& animation, GLfloat deltaTime)
	{
		if (!m_currentAnimation)
		{
			return;
		}

		const GLuint animationFrame = animation.currentFrame;

		if (animationFrame >= m_currentAnimation->frames.size())
		{
			return;
		}

		m_material->setTexture(m_currentAnimation->frames[animationFrame].texture);
	}

	EngineStatus SpriteAnimationPlayer::initialize(const BehaviourComponentConfig* config)
	{
		m_material = m_gameInstance->getRenderComponent<MaterialRenderComponent>("Material");

		if (!m_material)
			return { "SpriteAnimationPlayer::initialize failed. No material was assigned", true };

		const SpriteAnimationPlayerConfig* spriteAnimationConfig = dynamic_cast<const SpriteAnimationPlayerConfig*>(config);

		if (!spriteAnimationConfig)
			return { "Failed to convert BehaviourComponentConfig to SpriteAnimationPlayerConfig", true };

		m_spriteAnimations.clear();
		m_currentAnimation = nullptr;

		for (const auto& animationConfig : spriteAnimationConfig->getAnimations())
		{
			const std::string& animationName				= animationConfig.name;
			const auto& animation		= SpriteSheetLoader::getInstance()->getSpriteSheet(animationName);

			SpriteAnimation spriteAnimation		= animation;
			spriteAnimation.name				= animationName;
			spriteAnimation.durationS			= animationConfig.durationS;
			spriteAnimation.interFrameDelayS	= animationConfig.interFrameDelayS;
			spriteAnimation.timeBetweenFrames	= ( animationConfig.durationS / animation.frames.size() ) + spriteAnimation.interFrameDelayS;

			m_spriteAnimations.insert({ animationName, spriteAnimation });
		}

		return { };
	}

	const char* SpriteAnimationPlayer::getName() const
	{
		return "SpriteAnimationPlayer";
	}

	void SpriteAnimationPlayer::update(GLfloat deltaTime)
	{
		if ((m_animationState & AnimationState::PLAYING) == AnimationState::PLAYING)
		{
			if (!m_animationQueue.empty())
			{
				SpriteAnimation& playingAnimation	= m_animationQueue.front();
				unsigned int& currentFrame			= playingAnimation.currentFrame;

				if (playingAnimation.durationS <= 0.0f)
				{
					animate(playingAnimation, deltaTime);
					m_animationQueue.pop_front();
					m_playTime = 0.0f;
				}
				else if (m_playTime >= playingAnimation.durationS)
				{
					m_animationQueue.pop_front();
					m_playTime = 0.0f;

					if (!m_animationQueue.empty())
					{
						m_currentAnimation = &m_animationQueue.front();
					}
				}
				else if (m_playTime >= playingAnimation.timeBetweenFrames * currentFrame)
				{
					animate(playingAnimation, deltaTime);
					++currentFrame;
				}

				m_playTime += deltaTime;
			}
			else
			{
				m_animationState &= ~(AnimationState::PLAYING | AnimationState::PAUSED);
			}
		}
	}

	void SpriteAnimationPlayer::play(SpriteAnimation& animation, bool immediate /* = false */)
	{
		if (immediate)
		{
			stop();

			m_currentAnimation = &animation;
			m_currentAnimation->currentFrame = 0;
		}

		m_animationQueue.push_back(animation);
		m_animationState = AnimationState::PLAYING;
	}

	bool SpriteAnimationPlayer::isPlaying(const std::string& name) const
	{
		return !m_animationQueue.empty() && m_animationQueue.front().name == name;
	}

	void SpriteAnimationPlayer::pause()
	{
		m_animationState |= AnimationState::PAUSED;
	}

	void SpriteAnimationPlayer::resume()
	{
		m_animationState &= ~AnimationState::PAUSED;
	}

	void SpriteAnimationPlayer::stop()
	{
		if (!m_animationQueue.empty())
		{
			m_animationQueue.pop_front();
		}

		m_playTime			= 0.0f;
		m_animationState	&= ~(AnimationState::PLAYING|AnimationState::PAUSED);
	}
}