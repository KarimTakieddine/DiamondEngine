#pragma once

#include <memory>

#include <GL/glew.h>

#include "EngineStatus.h"

namespace diamond_engine
{
	class BehaviourComponentConfig;
	class GameInstance;
	class BehaviourComponent
	{
	public:
		virtual ~BehaviourComponent() = default;

		virtual const char* getName() const = 0;
		virtual void update(GLfloat deltaTime) = 0;
		virtual EngineStatus initialize(const std::unique_ptr<GameInstance>& instance, const BehaviourComponentConfig* config) = 0;
	};
}
