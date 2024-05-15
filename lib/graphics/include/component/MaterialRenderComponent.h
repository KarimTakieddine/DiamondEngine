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

		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override { return {  }; };
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;

		void setTexture(const Texture& texture);
		const Texture& getTexture() const;
		Texture& getTexture();

		void setColor(const glm::vec3& color);
		const glm::vec3& getColor() const;
		glm::vec3& getColor();

		void setTextureOffset(const glm::vec2& textureOffset);
		const glm::vec2& getTextureOffset() const;
		glm::vec2& getTextureOffset();

		EngineStatus requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus formatDrawCall(DrawCall* drawCall) final override;
		EngineStatus uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;

	private:
		UniformVec3* m_colorMemory{ nullptr };
		UniformVec2* m_textureOffsetMemory{ nullptr };
		DrawCall* m_drawCall{ nullptr };
	};
}
