#pragma once

#include <memory>
#include <vector>

#include "IRenderComponent.h"

namespace diamond_engine
{
	class IRenderComponent;
	class RenderObject;
	class GameInstance
	{
	public:
		void setRenderObject(RenderObject* renderObject);
		void setRenderComponents(std::vector<std::unique_ptr<IRenderComponent>> renderComponents);

	private:
		std::vector<std::unique_ptr<IRenderComponent>> m_renderComponents;
		RenderObject* m_renderObject{ nullptr };
	};
}
