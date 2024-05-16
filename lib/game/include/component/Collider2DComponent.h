#pragma once

#include <GL/glew.h>

#include "BehaviourComponent.h"
#include "ColliderType.h"
#include "Size.h"

namespace diamond_engine
{
	class TransformRenderComponent;
	class GameInstance;
	class Collider2DComponent : public BehaviourComponent
	{
	public:
		~Collider2DComponent() final override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		EngineStatus initialize(const BehaviourComponentConfig* config) final override;

		GameInstance* getTarget() const;
		void setTarget(GameInstance* target);

		GameInstance* getSource() const;
		void setSource(GameInstance* source);

		const Size& getSize() const;
		void setSize(const Size& size);

		ColliderType getType() const;
		void setType(ColliderType type);

		void onCollisionEnter2D(const glm::vec2& resolution, const std::string& name) final override;
		void onCollisionExit2D(const std::string& name) final override;

	private:
		Size m_size{ 1, 1 };
		GameInstance* m_target{ nullptr };
		GameInstance* m_source{ nullptr };
		ColliderType m_type{ ColliderType::OBSTACLE };
	};
}
