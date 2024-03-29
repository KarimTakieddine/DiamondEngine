#include "AnimationPlayer.h"

namespace diamond_engine
{
	void AnimationPlayer::run(GLfloat deltaTime)
	{
		if ((m_animationState & AnimationState::PLAYING) == AnimationState::PLAYING)
		{
			if (!m_animationQueue.empty())
			{
				Animation& playingAnimation	= m_animationQueue.front();
				unsigned int& currentFrame	= playingAnimation.frame;

				if (m_playTime >= playingAnimation.duration)
				{
					m_animationQueue.pop_front();
					m_playTime = 0.0f;
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

	void AnimationPlayer::play(const Animation& animation, bool immediate /* = false */)
	{
		if (immediate)
		{
			stop();
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
		if (!m_animationQueue.empty())
		{
			m_animationQueue.pop_front();
		}

		m_playTime			= 0.0f;
		m_animationState	&= ~(AnimationState::PLAYING|AnimationState::PAUSED);
	}
}