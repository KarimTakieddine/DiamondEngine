#pragma once

#include <deque>
#include <string>
#include <type_traits>

#include "Animation.h"
#include "BehaviourComponent.h"

namespace diamond_engine
{
	enum class AnimationState : unsigned char
	{
		STOPPED = 0,
		PLAYING = 1,
		PAUSED	= 1 << 1
	};

	inline AnimationState operator|(AnimationState a, AnimationState b)
	{
		return static_cast<AnimationState>(static_cast<std::underlying_type_t<AnimationState>>(a) | static_cast<std::underlying_type_t<AnimationState>>(b));
	}

	inline AnimationState& operator|=(AnimationState& l, AnimationState r)
	{
		l = l | r;
		return l;
	}

	inline AnimationState operator&(AnimationState a, AnimationState b)
	{
		return static_cast<AnimationState>(static_cast<std::underlying_type_t<AnimationState>>(a) & static_cast<std::underlying_type_t<AnimationState>>(b));
	}

	inline AnimationState& operator&=(AnimationState& l, AnimationState r)
	{
		l = l & r;
		return l;
	}

	inline AnimationState operator~(AnimationState state)
	{
		return static_cast<AnimationState>(~static_cast<std::underlying_type_t<AnimationState>>(state));
	}

	class AnimationPlayer : public BehaviourComponent
	{
	public:
		virtual ~AnimationPlayer() override = default;

		virtual void animate(const Animation& animation, GLfloat deltaTime) = 0;

		void update(GLfloat deltaTime) final override;

		void play(const Animation& animation, bool immediate = false);
		void pause();
		void resume();
		void stop();

	private:
		std::deque<Animation> m_animationQueue;
		AnimationState m_animationState{ AnimationState::STOPPED };
		float m_playTime{ 0.0f };
	};
}
