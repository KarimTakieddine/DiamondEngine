#pragma once

#include "RenderObject.h"

namespace diamond_engine
{
	class BehaviourComponent
	{
	public:
		virtual ~BehaviourComponent() = default;
		void setRenderObject(RenderObject* renderObject) { m_renderObject = renderObject; }
		RenderObject* getRenderObject() const { return m_renderObject; }

		virtual const char* getName() const = 0;
		virtual void update(GLfloat deltaTime) = 0;

	protected:
		RenderObject* m_renderObject{ nullptr };
	};
}
