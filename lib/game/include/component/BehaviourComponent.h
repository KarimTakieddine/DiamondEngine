#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/vec2.hpp>

#include "EngineStatus.h"

namespace diamond_engine
{
	class GameInstance;
	class BehaviourComponentConfig;
	class BehaviourComponent
	{
	public:
		virtual ~BehaviourComponent() = default;

		virtual const char* getName() const = 0;
		virtual void update(GLfloat deltaTime) = 0;
		virtual EngineStatus initialize(const BehaviourComponentConfig* config) = 0;

		void setGameInstance(GameInstance* gameInstance) { m_gameInstance = gameInstance; }
		virtual void onCollisionEnter2D(const glm::vec2& resolution, const std::string& name) { };
		virtual void onCollisionExit2D(const std::string& name) { };

	protected:
		GameInstance* m_gameInstance{ nullptr };
	};
}
