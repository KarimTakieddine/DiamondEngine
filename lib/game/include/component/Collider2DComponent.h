#pragma once

#include "BehaviourComponent.h"
#include "ColliderType.h"
#include "Size.h"

namespace diamond_engine
{
	class Collider2DComponent : public BehaviourComponent
	{
	public:
		~Collider2DComponent() final override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;

		RenderObject* getTarget() const;
		void setTarget(RenderObject* target);

		const Size& getSize() const;
		void setSize(const Size& size);

		ColliderType getType() const;
		void setType(ColliderType type);

	private:
		RenderObject* m_target{ nullptr };
		Size m_size{ 1, 1 };
		ColliderType m_type{ ColliderType::OBSTACLE };
	};
}
