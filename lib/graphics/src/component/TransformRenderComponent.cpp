#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "ShaderProgram.h"
#include "Transform.h"
#include "TransformComponentConfig.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	EngineStatus TransformRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
			return { "TransformRenderComponent::bindToShaderProgram - No shared shader program was provided", true };

		if (!m_localToWorld)
			return { "TransformRenderComponent::bindToShaderProgram - Local to world memory is NULL", true };

		m_localToWorld->memory.location	= shaderProgram->GetUniform("modelLocalToWorld");

		if (!m_localRotation)
			return { "TransformRenderComponent::bindToShaderProgram - Local rotation memory is NULL", true };

		m_localRotation->memory.location = shaderProgram->GetUniform("modelLocalRotation");

		if (!m_localScale)
			return { "TransformRenderComponent::bindToShaderProgram - Local scale memory is NULL", true };

		m_localScale->memory.location = shaderProgram->GetUniform("modelLocalScale");

		return { };
	}

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

	EngineStatus TransformRenderComponent::requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		m_localToWorld		= memoryPool->requestMemory<UniformMat4>({ { glm::mat4(1.0f), 1 } });
		m_localRotation		= memoryPool->requestMemory<UniformMat4>({ { glm::mat4(1.0f), 1 } });
		m_localScale		= memoryPool->requestMemory<UniformMat4>({ { glm::mat4(1.0f), 1 } });
		m_positionUniform	= memoryPool->requestMemory<UniformVec3>({ { { 0.0f, 0.0f, 0.0f }, 1 } });

		return { };
	}

	EngineStatus TransformRenderComponent::releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		memoryPool->freeMemory(sizeof(UniformVec3));
		m_positionUniform = nullptr;

		memoryPool->freeMemory(sizeof(UniformMat4));
		m_localScale = nullptr;

		memoryPool->freeMemory(sizeof(UniformMat4));
		m_localRotation = nullptr;

		memoryPool->freeMemory(sizeof(UniformMat4));
		m_localToWorld = nullptr;

		return { };
	}

	EngineStatus TransformRenderComponent::onDrawCallAllocated(DrawCall* drawCall) { return { }; }

	EngineStatus TransformRenderComponent::uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		uploadUniformMemory(m_localToWorld);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		uploadUniformMemory(m_localRotation);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		uploadUniformMemory(m_localScale);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		// Skip position upload (no shader position uniform)

		memoryPool->advanceSeek(sizeof(UniformVec3));

		return { };
	}

	EngineStatus TransformRenderComponent::initialize(const RenderComponentConfig* config)
	{
		const TransformComponentConfig* transformConfig = dynamic_cast<const TransformComponentConfig*>(config);

		if (!transformConfig)
			return { "TransformRenderComponent::initialize failed. Could not convert config parameter to target type" ,true};

		setPosition(transformConfig->getPosition());
		setLocalScale(transformConfig->getLocalScale());

		return { };
	}
}