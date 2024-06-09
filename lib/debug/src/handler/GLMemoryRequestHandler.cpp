#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "GLMemoryRequest.h"
#include "GLMemoryRequestHandler.h"

namespace diamond_engine
{
	std::string GLMemoryRequestHandler::doHandleEvent(const std::unique_ptr<DebugEvent>& e) const
	{
		std::stringstream stream;

		const GLMemoryRequest* memoryRequestEvent = dynamic_cast<const GLMemoryRequest*>(e.get());
		if (!memoryRequestEvent)
			throw std::runtime_error("GLMemoryRequestHandler failed to convert event to target type");

		stream
			<< "GL mem req "
			<< std::hex << memoryRequestEvent->getLocation()
			<< " value " << std::to_string(memoryRequestEvent->getValue()) << std::endl;

		std::string result = stream.str();
		std::transform(
			result.cbegin(),
			result.cend(),
			result.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return result;
	}
}