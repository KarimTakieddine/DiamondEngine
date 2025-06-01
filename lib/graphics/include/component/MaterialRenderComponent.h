#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "IRenderComponent.h"
#include "Texture.h"
#include "UniformMemory.hpp"

namespace diamond_engine
{
	class RenderMaterial;
	class MaterialRenderComponent : public IRenderComponent
	{
	public:
		~MaterialRenderComponent() final override = default;

		const char* getName() const final override { return "Material"; }

		void setTexture(GLuint texture);

		void setColor(const glm::vec3& color);
		const glm::vec3& getColor() const;

		void setTextureOffset(const glm::vec2& textureOffset);
		const glm::vec2& getTextureOffset() const;
		glm::vec2& getTextureOffset();

		EngineStatus onDrawCallAllocated(DrawCall* drawCall) final override;
		EngineStatus initialize(const RenderComponentConfig* config) final override;

		RenderDescriptor getRenderDescriptor() const final override;
		void onGraphicsMemoryAllocated(GLubyte* allocatedMemory) final override;

	private:
		UniformVec3* m_colorMemory{ nullptr };
		UniformVec2* m_textureOffsetMemory{ nullptr };
		DrawCall* m_drawCall{ nullptr };
	};
}
