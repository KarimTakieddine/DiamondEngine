#include "GameInstance.h"
#include "Collider2DComponent.h"
#include "Collider2DComponentConfig.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	const char* Collider2DComponent::getName() const
	{
		return "Collider2D";
	}

	void Collider2DComponent::update(GLfloat deltaTime)
	{
		if (!m_source || !m_target)
		{
			return;
		}

		const glm::mat4& targetLocalScale = m_target->getLocalScale();
		const glm::vec2 targetScale{ targetLocalScale[0].x, targetLocalScale[1].y };

		m_source->setLocalScale(
			{
				static_cast<GLfloat>(m_size.width) * targetScale.x,
				static_cast<GLfloat>(m_size.height) * targetScale.y,
				1.0f
			});

		m_source->setPosition(m_target->getPosition());
	}

	EngineStatus Collider2DComponent::initialize(const BehaviourComponentConfig* config)
	{
		const Collider2DComponentConfig* componentConfig = dynamic_cast<const Collider2DComponentConfig*>(config);

		if (!componentConfig)
			return { "Collider2DComponent::initialize failed. Could not convert config to target type", true };

		setSize(componentConfig->getSize());
		setType(componentConfig->getType());

		return { };
	}

	TransformRenderComponent* Collider2DComponent::getTarget() const
	{
		return m_target;
	}

	void Collider2DComponent::setTarget(TransformRenderComponent* target)
	{
		m_target = target;
	}

	TransformRenderComponent* Collider2DComponent::getSource() const
	{
		return m_source;
	}

	void Collider2DComponent::setSource(TransformRenderComponent* source)
	{
		m_source = source;
	}

	const Size& Collider2DComponent::getSize() const
	{
		return m_size;
	}

	void Collider2DComponent::setSize(const Size& size)
	{
		m_size = size;
	}

	ColliderType Collider2DComponent::getType() const
	{
		return m_type;
	}

	void Collider2DComponent::setType(ColliderType type)
	{
		m_type = type;
	}
}