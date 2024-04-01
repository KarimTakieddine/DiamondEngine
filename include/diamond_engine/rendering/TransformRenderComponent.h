#pragma once

#include <glm/vec3.hpp>

#include "IRenderComponent.h"

namespace diamond_engine
{
	class Transform;
	class TransformRenderComponent : public IRenderComponent
	{
	public:
		~TransformRenderComponent() final override = default;

		RenderComponentType getComponentType() const final override
		{
			return RenderComponentType::TRANSFORM;
		}

		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override;
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;

		void setPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		Transform* m_transform{ nullptr };
		GLint m_localToWorldLocation{ -1 };
		GLint m_localRotationLocation{ -1 };
		GLint m_localScaleLocation{ -1 };
	};
}
