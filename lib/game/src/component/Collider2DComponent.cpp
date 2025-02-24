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

		m_source->setLocalScale(
			{
				m_size,
				1.0f
			});

		m_source->setPosition(m_target->getPosition() + glm::vec3{ m_offset, 0.0f });
	}

	EngineStatus Collider2DComponent::initialize(const BehaviourComponentConfig* config)
	{
		const Collider2DComponentConfig* componentConfig = dynamic_cast<const Collider2DComponentConfig*>(config);

		if (!componentConfig)
			return { "Collider2DComponent::initialize failed. Could not convert config to target type", true };

		setSize(componentConfig->getSize());
		setOffset(componentConfig->getOffset());
		setType(componentConfig->getType());
		setIgnoreFlags(componentConfig->getIgnoreFlags());

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

	const glm::vec2& Collider2DComponent::getSize() const
	{
		return m_size;
	}

	void Collider2DComponent::setSize(const glm::vec2& size)
	{
		m_size = size;
	}

	const glm::vec2& Collider2DComponent::getOffset() const
	{
		return m_offset;
	}

	void Collider2DComponent::setOffset(const glm::vec2& offset)
	{
		m_offset = offset;
	}

	ColliderType Collider2DComponent::getType() const
	{
		return m_type;
	}

	void Collider2DComponent::setType(ColliderType type)
	{
		m_type = type;
	}

	ColliderIgnoreFlags Collider2DComponent::getIgnoreFlags() const
	{
		return m_ignoreFlags;
	}

	void Collider2DComponent::setIgnoreFlags(ColliderIgnoreFlags flags)
	{
		m_ignoreFlags = flags;
	}
}