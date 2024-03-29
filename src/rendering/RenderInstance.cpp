#include "RenderInstance.h"

namespace diamond_engine
{
	void RenderInstance::setObject(RenderObject* object)
	{
		m_object = object;
	}

	void RenderInstance::setComponents(std::vector<std::unique_ptr<IRenderComponent>> components)
	{
		m_components = std::move(components);
	}
}