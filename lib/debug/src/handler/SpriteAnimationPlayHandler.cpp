#include <algorithm>
#include <sstream>

#include "SpriteAnimationPlay.h"
#include "SpriteAnimationPlayHandler.h"

namespace diamond_engine
{
	std::string SpriteAnimationPlayHandler::doHandleEvent(const std::unique_ptr<DebugEvent>& e) const
	{
		std::stringstream stream;

		const SpriteAnimationPlay* spriteAnimationPlayEvent = dynamic_cast<const SpriteAnimationPlay*>(e.get());
		if (!spriteAnimationPlayEvent)
			throw std::runtime_error("SpriteAnimationPlayHandler failed to convert event to target type");

		stream
			<< "SP ani ply "
			<< spriteAnimationPlayEvent->getName()
			<< " inst "
			<< ( spriteAnimationPlayEvent->immediate() ? "true" : "false" ) << std::endl;

		std::string result = stream.str();
		std::transform(
			result.cbegin(),
			result.cend(),
			result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return result;
	}
}