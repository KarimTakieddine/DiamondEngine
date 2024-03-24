#include "AnimationPlayer.h"

namespace diamond_engine
{
	void AnimationPlayer::run(float deltaTime)
	{
		if ((m_animationState & AnimationState::PLAYING) == AnimationState::PLAYING)
		{
			if (!m_animationQueue.empty())
			{
				Animation& playingAnimation	= m_animationQueue.front();
				unsigned int& currentFrame	= playingAnimation.frame;

				if (m_playTime >= playingAnimation.duration)
				{
					stop();
				}
				else if (m_playTime >= (playingAnimation.timeBetweenFrames * currentFrame + playingAnimation.delayBetweenFrames))
				{
					animate(playingAnimation, deltaTime);
					++currentFrame;
				}

				m_playTime += deltaTime;
			}
		}
	}

	void AnimationPlayer::play(const Animation& animation, bool immediate /* = false */)
	{
		if (immediate)
		{
			m_animationQueue.clear();
		}

		m_animationQueue.push_back(animation);
		m_animationState = AnimationState::PLAYING;
	}

	void AnimationPlayer::pause()
	{
		m_animationState |= AnimationState::PAUSED;
	}

	void AnimationPlayer::resume()
	{
		m_animationState &= ~AnimationState::PAUSED;
	}

	void AnimationPlayer::stop()
	{
		m_animationQueue.pop_front();

		m_playTime			= 0.0f;
		m_animationState	&= ~(AnimationState::PLAYING|AnimationState::PAUSED);
	}
}