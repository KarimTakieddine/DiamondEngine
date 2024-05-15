#pragma once

#include <glm/vec3.hpp>

#include "IRenderComponent.h"
#include "UniformMemory.hpp"

namespace diamond_engine
{
	class Transform;
	class TransformRenderComponent : public IRenderComponent
	{
	public:
		~TransformRenderComponent() final override = default;

		const char* getName() const final override { return "Transform"; }

		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override;
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;

		void setPosition(const glm::vec3& position);

		EngineStatus requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus formatDrawCall(DrawCall* drawCall) final override;
		EngineStatus uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;

	private:
		// glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		/*
		Transform* m_transform{ nullptr };
		GLint m_localToWorldLocation{ -1 };
		GLint m_localRotationLocation{ -1 };
		GLint m_localScaleLocation{ -1 };
		*/
		UniformMat4* m_localToWorld{ nullptr };
		UniformMat4* m_localRotation{ nullptr };
		UniformMat4* m_localScale{ nullptr };
		UniformVec3* m_positionUniform{ nullptr };
	};
}
