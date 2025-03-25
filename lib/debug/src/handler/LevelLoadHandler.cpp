#include <algorithm>
#include <sstream>

#include "LevelLoad.h"
#include "LevelLoadHandler.h"

namespace diamond_engine
{
	std::string LevelLoadHandler::doHandleEvent(const std::unique_ptr<DebugEvent>& e) const
	{
		std::stringstream stream;

		const LevelLoad* levelLoadEvent = dynamic_cast<const LevelLoad*>(e.get());
		if (!levelLoadEvent)
			throw std::runtime_error("LevelLoadHandler failed to convert event to target type");

		stream
			<< "Level load "
			<< levelLoadEvent->getName()
			<< std::endl;

		std::string result = stream.str();
		std::transform(
			result.cbegin(),
			result.cend(),
			result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return result;
	}
}