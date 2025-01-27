#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/vec2.hpp>

#include "EngineStatus.h"

namespace diamond_engine
{
	class GameInstance;
	class BehaviourComponentConfig;
	class Collider2DComponent;
	class BehaviourComponent
	{
	public:
		virtual ~BehaviourComponent() = default;

		virtual const char* getName() const = 0;
		virtual void update(GLfloat deltaTime) = 0;
		virtual EngineStatus initialize(const BehaviourComponentConfig* config) = 0;

		void setGameInstance(GameInstance* gameInstance) { m_gameInstance = gameInstance; }
		GameInstance* getGameInstance() { return m_gameInstance; }

		virtual void onCollisionEnter2D(const glm::vec2& resolution, GameInstance* gameInstance, Collider2DComponent* collider2D) { };
		virtual void onCollisionExit2D(GameInstance* gameInstance, Collider2DComponent* collider2D) { };

	protected:
		GameInstance* m_gameInstance{ nullptr };
	};
}
