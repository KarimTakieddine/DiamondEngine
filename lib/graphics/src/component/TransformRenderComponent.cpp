#include "RenderObject.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "TransformComponentConfig.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	std::vector<RenderUpload> TransformRenderComponent::getUploads() const
	{
		return {
			/*
			{ &m_transform->getLocalToWorld(),	RenderUploadType::Matrix4, m_localToWorldLocation },
			{ &m_transform->getLocalRotation(), RenderUploadType::Matrix4, m_localRotationLocation },
			{ &m_transform->getLocalScale(),	RenderUploadType::Matrix4, m_localScaleLocation }
			*/
		};
	}

	EngineStatus TransformRenderComponent::onDrawCallRegistered(RenderDrawCall* renderDrawCall) { return { }; }

	EngineStatus TransformRenderComponent::onRenderObjectAllocated(RenderObject* renderObject)
	{
		if (!renderObject)
		{
			return { "TransformRenderComponent::onRenderObjectAllocated - No RenderObject instance was provided", true };
		}

		//m_transform = &renderObject->transform;

		// setPosition(m_position);

		return { };
	}

	EngineStatus TransformRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
		{
			return { "TransformRenderComponent::bindToShaderProgram - No shared shader program was provided", true };
		}

		if (!m_localToWorld)
		{
			return { "TransformRenderComponent::bindToShaderProgram - Local to world memory is NULL", true };
		}

		m_localToWorld->memory.location	= shaderProgram->GetUniform("modelLocalToWorld");

		if (!m_localRotation)
		{
			return { "TransformRenderComponent::bindToShaderProgram - Local rotation memory is NULL", true };
		}

		m_localRotation->memory.location = shaderProgram->GetUniform("modelLocalRotation");

		if (!m_localScale)
		{
			return { "TransformRenderComponent::bindToShaderProgram - Local scale memory is NULL", true };
		}

		m_localScale->memory.location = shaderProgram->GetUniform("modelLocalScale");

		/*m_localToWorldLocation	= shaderProgram->GetUniform("modelLocalToWorld");
		m_localRotationLocation = shaderProgram->GetUniform("modelLocalRotation");
		m_localScaleLocation	= shaderProgram->GetUniform("modelLocalScale");*/

		return { };
	}

	void TransformRenderComponent::setPosition(const glm::vec3& position)
	{
		/*if (m_transform)
		{
			m_transform->SetPosition(position);
		}*/

		if (m_positionUniform)
		{
			m_positionUniform->memory.value = position;
		}
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
		// TODO: Upload m_localToWorld

		uploadUniformMemory(m_localToWorld);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		// TODO: Upload m_localRotation

		uploadUniformMemory(m_localRotation);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		// TODO: Upload m_localScale

		uploadUniformMemory(m_localScale);
		memoryPool->advanceSeek(sizeof(UniformMat4));

		// Skip position upload (no position)

		memoryPool->advanceSeek(sizeof(UniformVec3));

		return { };
	}

	EngineStatus TransformRenderComponent::initialize(const RenderComponentConfig* config)
	{
		const TransformComponentConfig* transformConfig = dynamic_cast<const TransformComponentConfig*>(config);

		if (!transformConfig)
			return { "TransformRenderComponent::initialize failed. Could not convert config parameter to target type" ,true};

		setPosition(transformConfig->getPosition());

		return { };
	}
}
