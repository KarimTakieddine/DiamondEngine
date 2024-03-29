#pragma once

#include <memory>

#include "IRenderComponent.h"
#include "RenderObject.h"

namespace diamond_engine
{
	class RenderInstance
	{
	public:
		void setObject(RenderObject* object);
		void setComponents(std::vector<std::unique_ptr<IRenderComponent>> components);

	protected:
		std::vector<std::unique_ptr<IRenderComponent>> m_components;
		RenderObject* m_object{ nullptr };
	};
}
