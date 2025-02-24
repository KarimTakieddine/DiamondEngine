#pragma once

#include <GL/glew.h>

#include <glm/vec2.hpp>

#include "BehaviourComponent.h"
#include "ColliderIgnoreFlags.h"
#include "ColliderType.h"

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

		TransformRenderComponent* getTarget() const;
		void setTarget(TransformRenderComponent* target);

		TransformRenderComponent* getSource() const;
		void setSource(TransformRenderComponent* source);

		const glm::vec2& getSize() const;
		void setSize(const glm::vec2& size);

		const glm::vec2& getOffset() const;
		void setOffset(const glm::vec2& offset);

		ColliderType getType() const;
		void setType(ColliderType type);

		ColliderIgnoreFlags getIgnoreFlags() const;
		void setIgnoreFlags(ColliderIgnoreFlags flags);

	private:
		glm::vec2 m_size{ 1.0f, 1.0f };
		glm::vec2 m_offset{ 0.0f, 0.0f };
		TransformRenderComponent* m_target{ nullptr };
		TransformRenderComponent* m_source{ nullptr };
		ColliderType m_type{ ColliderType::OBSTACLE };
		ColliderIgnoreFlags m_ignoreFlags{ ColliderIgnoreFlags::NONE };
	};
}
