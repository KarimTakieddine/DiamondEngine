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

		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;

		void setPosition(const glm::vec3& position);

		EngineStatus requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus onDrawCallAllocated(DrawCall* drawCall) final override;
		EngineStatus uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus initialize(const RenderComponentConfig* config) final override;

	private:
		UniformMat4* m_localToWorld{ nullptr };
		UniformMat4* m_localRotation{ nullptr };
		UniformMat4* m_localScale{ nullptr };
		UniformVec3* m_positionUniform{ nullptr };
	};
}
