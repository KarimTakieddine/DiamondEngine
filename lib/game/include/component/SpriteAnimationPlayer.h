#pragma once

#include <deque>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "SpriteAnimation.h"
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

	class MaterialRenderComponent;
	class SpriteAnimationPlayer : public BehaviourComponent
	{
	public:
		virtual ~SpriteAnimationPlayer() override = default;

		virtual void animate(const SpriteAnimation& animation, GLfloat deltaTime);

		const char* getName() const final override;
		EngineStatus SpriteAnimationPlayer::initialize(const BehaviourComponentConfig* config) final override;
		void update(GLfloat deltaTime) final override;

		void play(SpriteAnimation& animation, bool immediate = false);
		void playAnimation(const std::string& name, bool immediate = false);
		bool isPlaying(const std::string& name) const;
		void pause();
		void resume();
		void stop();

	private:
		std::unordered_map<std::string, SpriteAnimation> m_spriteAnimations;
		MaterialRenderComponent* m_material{ nullptr };
		SpriteAnimation* m_currentAnimation{ nullptr };
		std::deque<SpriteAnimation> m_animationQueue;
		AnimationState m_animationState{ AnimationState::STOPPED };
		float m_playTime{ 0.0f };
	};
}
