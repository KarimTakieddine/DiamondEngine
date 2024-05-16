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

		TransformRenderComponent* getTarget() const;
		void setTarget(TransformRenderComponent* target);

		TransformRenderComponent* getSource() const;
		void setSource(TransformRenderComponent* source);

		const Size& getSize() const;
		void setSize(const Size& size);

		ColliderType getType() const;
		void setType(ColliderType type);

		void bindSourceToTarget(const std::unique_ptr<GameInstance>& source, const std::unique_ptr<GameInstance>& target);

	private:
		TransformRenderComponent* m_target{ nullptr };
		TransformRenderComponent* m_source{ nullptr };
		Size m_size{ 1, 1 };
		ColliderType m_type{ ColliderType::OBSTACLE };
	};
}
