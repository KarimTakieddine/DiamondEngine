#include "GLAllocationEvent.h"
#include "GLAllocationEventHandler.h"

namespace diamond_engine
{
	std::stringstream GLAllocationEventHandler::doHandleEvent(const std::unique_ptr<DebugEvent>& e) const
	{
		std::stringstream result;

		const GLAllocationEvent* allocationEvent = dynamic_cast<const GLAllocationEvent*>(e.get());
		if (!allocationEvent)
		{
			// result << "INVALID_EVENT" << std::endl; TODO: Needs extra font(s) support
			return result;
		}

		result << "hello" << std::endl;

		//result << std::hex << allocationEvent->getCurrentTop(); TODO: Needs extra font(s) support

		return result;
	}
}