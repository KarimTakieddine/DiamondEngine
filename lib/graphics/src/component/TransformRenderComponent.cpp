#include "RenderObject.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	std::vector<RenderUpload> TransformRenderComponent::getUploads() const
	{
		return {
			{ &m_transform->getLocalToWorld(),	RenderUploadType::Matrix4, m_localToWorldLocation },
			{ &m_transform->getLocalRotation(), RenderUploadType::Matrix4, m_localRotationLocation },
			{ &m_transform->getLocalScale(),	RenderUploadType::Matrix4, m_localScaleLocation }
		};
	}

	EngineStatus TransformRenderComponent::onDrawCallRegistered(RenderDrawCall* renderDrawCall) { return { }; }

	EngineStatus TransformRenderComponent::onRenderObjectAllocated(RenderObject* renderObject)
	{
		if (!renderObject)
		{
			return { "TransformRenderComponent::onRenderObjectAllocated - No RenderObject instance was provided", true };
		}

		m_transform = &renderObject->transform;

		setPosition(m_position);

		return { };
	}

	EngineStatus TransformRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
		{
			return { "TransformRenderComponent::bindToShaderProgram - No shared shader program was provided", true };
		}

		m_localToWorldLocation	= shaderProgram->GetUniform("modelLocalToWorld");
		m_localRotationLocation = shaderProgram->GetUniform("modelLocalRotation");
		m_localScaleLocation	= shaderProgram->GetUniform("modelLocalScale");

		return { };
	}

	void TransformRenderComponent::setPosition(const glm::vec3& position)
	{
		if (m_transform)
		{
			m_transform->SetPosition(position);
		}

		m_position = position;
	}
}
