#pragma once

#define GL_FORCE_SWIZZLE
#include <glm/glm.hpp>

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

		void setPosition(const glm::vec3& position);
		glm::vec3 getPosition() const;

		const glm::mat4& getLocalScale() const;
		void setLocalScale(const glm::vec3& scale);

		void translate(const glm::vec3& displacement);
		void translate(const glm::vec2& displacement);
		void rotate(GLfloat degrees, const glm::vec3& axis);

		const glm::mat4& getLocalRotation() const;
		void setLocalEulerAngles(const glm::vec3& localEulerAngles);

		EngineStatus onDrawCallAllocated(DrawCall* drawCall) final override;
		EngineStatus initialize(const RenderComponentConfig* config) final override;

		RenderDescriptor getRenderDescriptor() const final override;
		void onGraphicsMemoryAllocated(GLubyte* allocatedMemory) final override;

	private:
		UniformMat4* m_localToWorld{ nullptr };
		UniformMat4* m_localRotation{ nullptr };
		UniformMat4* m_localScale{ nullptr };
		UniformVec3* m_positionUniform{ nullptr };
	};
}
