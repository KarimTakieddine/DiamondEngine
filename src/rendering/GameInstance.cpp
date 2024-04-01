#include "GameInstance.h"

namespace diamond_engine
{
	void GameInstance::setRenderObject(RenderObject* renderObject)
	{
		m_renderObject = renderObject;
	}

	void GameInstance::setRenderComponents(std::vector<std::unique_ptr<IRenderComponent>> renderComponents)
	{
		m_renderComponents = std::move(renderComponents);
	}
}