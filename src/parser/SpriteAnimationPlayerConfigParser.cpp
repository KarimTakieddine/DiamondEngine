#include "AnimationParser.h"
#include "SpriteAnimationPlayerConfig.h"
#include "SpriteAnimationPlayerConfigParser.h"

namespace diamond_engine
{
	///* static */ std::unique_ptr<ComponentConfig> SpriteAnimationPlayerConfigParser::Parse(const pugi::xml_node& componentNode)
	//{
	//	std::unique_ptr<SpriteAnimationPlayerConfig> result = std::make_unique<SpriteAnimationPlayerConfig>();

	//	for (const auto& animationNode : componentNode.children("Animation"))
	//	{
	//		result->addAnimation(AnimationParser::Parse(animationNode));
	//	}

	//	return result;
	//}
}