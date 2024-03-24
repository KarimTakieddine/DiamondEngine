#include <stdexcept>

#include "SpriteAnimationPlayer.h"
#include "SpriteAnimationPlayerBuilder.h"
#include "SpriteAnimationPlayerConfig.h"

namespace diamond_engine
{
	std::unique_ptr<Component> SpriteAnimationPlayerBuilder::Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const ComponentConfig* componentConfig)
	{
		const SpriteAnimationPlayerConfig* config = dynamic_cast<const SpriteAnimationPlayerConfig*>(componentConfig);

		if (!config)
		{
			throw std::runtime_error(
				"Failed to convert ComponentConfig. Name: " +
				(componentConfig ? std::string(componentConfig->GetName()) : "NULL") +
				" to SpriteAnimationPlayerConfig");
		}

		std::unique_ptr<SpriteAnimationPlayer> result = std::make_unique<SpriteAnimationPlayer>();

		result->setAnimations(config->getAnimations());

		return result;
	}
}