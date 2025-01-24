#pragma once

#include <GL/glew.h>

#include <glm/vec2.hpp>

#include "BehaviourComponentConfig.h"
#include "ColliderType.h"

namespace diamond_engine
{
	class Collider2DComponentConfig : public BehaviourComponentConfig
	{
	public:
		~Collider2DComponentConfig() final override = default;
		const char* getName() const final override;

		const glm::vec2& getSize() const;
		void setSize(const glm::vec2& size);

		const glm::vec2& getOffset() const;
		void setOffset(const glm::vec2& offset);

		ColliderType getType() const;
		void setType(ColliderType type);

		GLsizei getTargetIndex() const;
		void setTargetIndex(GLsizei index);

	private:
		glm::vec2 m_size{ 1.0f, 1.0f };
		glm::vec2 m_offset{ 0.0f, 0.0f };
		ColliderType m_type{ ColliderType::OBSTACLE };
		GLsizei m_targetIndex{ -1 };
	};
}
