#pragma once

#include <GL/glew.h>

#include "BehaviourComponentConfig.h"
#include "ColliderType.h"
#include "Size.h"

namespace diamond_engine
{
	class Collider2DComponentConfig : public BehaviourComponentConfig
	{
	public:
		~Collider2DComponentConfig() final override = default;
		const char* getName() const final override;

		const Size& getSize() const;
		void setSize(const Size& size);

		ColliderType getType() const;
		void setType(ColliderType type);

		GLsizei getTargetIndex() const;
		void setTargetIndex(GLsizei index);

	private:
		Size m_size{ 1, 1 };
		ColliderType m_type{ ColliderType::OBSTACLE };
		GLsizei m_targetIndex{ -1 };
	};
}
