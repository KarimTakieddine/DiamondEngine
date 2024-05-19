#include <stdexcept>

#include "GameInstance.h"
#include "MaterialRenderComponent.h"
#include "SpriteSheetLoader.h"
#include "SpriteAnimationPlayer.h"
#include "SpriteAnimationPlayerConfig.h"

namespace diamond_engine
{
	void SpriteAnimationPlayer::playAnimation(const std::string& name, bool immediate /* = false */)
	{
		auto it = m_spriteSheetAnimations.find(name);

		if (it == m_spriteSheetAnimations.cend())
			throw std::runtime_error("Could not find sprite animation named: " + name);

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

		m_material->setTexture(m_currentSpriteSheet->frames[animationFrame]);
	}

	EngineStatus SpriteAnimationPlayer::initialize(const BehaviourComponentConfig* config)
	{
		m_material = m_gameInstance->getRenderComponent<MaterialRenderComponent>("Material");

		if (!m_material)
			return { "SpriteAnimationPlayer::initialize failed. No material was assigned", true };

		const SpriteAnimationPlayerConfig* spriteAnimationConfig = dynamic_cast<const SpriteAnimationPlayerConfig*>(config);

		if (!spriteAnimationConfig)
			return { "Failed to convert BehaviourComponentConfig to SpriteAnimationPlayerConfig", true };

		m_spriteSheetAnimations.clear();
		m_currentSpriteSheet = nullptr;

		for (const auto& animation : spriteAnimationConfig->getAnimations())
		{
			const std::string& spriteSheetName = animation.name;
			const auto& spriteSheet = SpriteSheetLoader::getInstance()->getSpriteSheet(spriteSheetName);

			Animation spriteAnimation = animation;
			spriteAnimation.timeBetweenFrames = ( animation.duration / spriteSheet.frames.size() ) + animation.delayBetweenFrames;

			m_spriteSheetAnimations.insert({ spriteSheetName, { spriteSheet, spriteAnimation  } });
		}

		return { };
	}

	const char* SpriteAnimationPlayer::getName() const
	{
		return "SpriteAnimationPlayer";
	}
}