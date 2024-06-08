#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "GLAllocationEvent.h"
#include "GLAllocationEventHandler.h"

namespace diamond_engine
{
	std::string GLAllocationEventHandler::doHandleEvent(const std::unique_ptr<DebugEvent>& e) const
	{
		std::stringstream stream;

		const GLAllocationEvent* allocationEvent = dynamic_cast<const GLAllocationEvent*>(e.get());
		if (!allocationEvent)
			throw std::runtime_error("GLAllocationEventHandler failed to convert event to target type");

		stream
		<< "GL mem alloc"	<< std::endl
		<< "prev top "		<< std::hex << allocationEvent->getCurrentTop()				<< std::endl
		<< "next top "		<< std::hex << allocationEvent->getNewTop()					<< std::endl
		<< "objs cnt "		<< std::to_string(allocationEvent->getObjectCount())	<< std::endl
		<< "ctop val "		<< std::to_string(*allocationEvent->getCurrentTop())	<< std::endl
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