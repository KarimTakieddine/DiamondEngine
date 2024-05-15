#pragma once

#include <GL/glew.h>

namespace diamond_engine
{
	class BehaviourComponent
	{
	public:
		virtual ~BehaviourComponent() = default;

		virtual const char* getName() const = 0;
		virtual void update(GLfloat deltaTime) = 0;

	protected:
		
	};
}
