#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "ShaderProgram.h"
#include "Transform.h"
#include "TransformComponentConfig.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	void TransformRenderComponent::setPosition(const glm::vec3& position)
	{
		if (!m_localToWorld)
			return;

		glm::vec4& currentPosition = m_localToWorld->memory.value[3];

		currentPosition.x = position.x;
		currentPosition.y = position.y;
		currentPosition.z = position.z;
	}

	glm::vec3 TransformRenderComponent::getPosition() const
	{
		return glm::xyz(m_localToWorld->memory.value * glm::vec4(m_positionUniform->memory.value, 1.0f));
	}

	const glm::mat4& TransformRenderComponent::getLocalScale() const
	{
		return m_localScale->memory.value;
	}

	void TransformRenderComponent::setLocalScale(const glm::vec3& scale)
	{
		if (!m_localScale)
			return;

		m_localScale->memory.value = glm::scale(glm::mat4(1.0f), scale);
	}

	void TransformRenderComponent::translate(const glm::vec3& displacement)
	{
		if (!m_localToWorld)
			return;

		m_localToWorld->memory.value = glm::translate(m_localToWorld->memory.value, displacement);
	}

	void TransformRenderComponent::translate(const glm::vec2& displacement)
	{
		translate(glm::vec3{ displacement, 0.0f });
	}

	void TransformRenderComponent::rotate(GLfloat degrees, const glm::vec3& axis)
	{
		if (!m_localToWorld)
			return;

		m_localToWorld->memory.value = glm::rotate(m_localToWorld->memory.value, glm::radians(degrees), axis);
	}

	const glm::mat4& TransformRenderComponent::getLocalRotation() const
	{
		return m_localRotation->memory.value;
	}

	void TransformRenderComponent::setLocalEulerAngles(const glm::vec3& localEulerAngles)
	{
		if (!m_localRotation)
			return;

		m_localRotation->memory.value = glm::eulerAngleXYZ(
			glm::radians(localEulerAngles.x),
			glm::radians(localEulerAngles.y),
			glm::radians(localEulerAngles.z));
	}

	EngineStatus TransformRenderComponent::onDrawCallAllocated(DrawCall* drawCall) { return { }; }

	EngineStatus TransformRenderComponent::initialize(const RenderComponentConfig* config)
	{
		const TransformComponentConfig* transformConfig = dynamic_cast<const TransformComponentConfig*>(config);

		if (!transformConfig)
			return { "TransformRenderComponent::initialize failed. Could not convert config parameter to target type" ,true};

		setPosition(transformConfig->getPosition());
		setLocalScale(transformConfig->getLocalScale());

		return { };
	}

	RenderDescriptor TransformRenderComponent::getRenderDescriptor() const
	{
		return
		{
			{
				{
					"modelLocalToWorld",
					1,
					UniformDescriptor::Type::MATRIX_4
				},
				{
					"modelLocalRotation",
					1,
					UniformDescriptor::Type::MATRIX_4
				},
				{
					"modelLocalScale",
					1,
					UniformDescriptor::Type::MATRIX_4
				},
				{
					"",
					1,
					UniformDescriptor::Type::VECTOR_3
				}
			}
		};
	}

	void TransformRenderComponent::onGraphicsMemoryAllocated(GLubyte* allocatedMemory)
	{
		m_localToWorld		= reinterpret_cast<UniformMat4*>(allocatedMemory);
		m_localRotation		= reinterpret_cast<UniformMat4*>(allocatedMemory + sizeof(UniformMat4));
		m_localScale		= reinterpret_cast<UniformMat4*>(allocatedMemory + ( 2 * sizeof(UniformMat4) ));
		m_positionUniform	= reinterpret_cast<UniformVec3*>(allocatedMemory + ( 3 * sizeof(UniformMat4) ));

		m_localToWorld->memory.value	= glm::mat4(1.0f);
		m_localRotation->memory.value	= glm::mat4(1.0f);
		m_localScale->memory.value		= glm::mat4(1.0f);
		m_positionUniform->memory.value	= { 0.0f, 0.0f, 0.0f };
	}
}