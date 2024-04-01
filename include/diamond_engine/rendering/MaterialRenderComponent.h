#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "IRenderComponent.h"
#include "Texture.h"

namespace diamond_engine
{
	class RenderMaterial;
	class MaterialRenderComponent : public IRenderComponent
	{
	public:
		~MaterialRenderComponent() final override = default;

		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override;
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

	private:
		Texture m_texture;
		glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
		glm::vec2 m_textureOffset{ 0.0f, 0.0f };
		RenderMaterial* m_material{ nullptr };
		RenderDrawCall* m_renderDrawCall{ nullptr };
		GLint m_colorUniformLocation{ -1 };
		GLint m_textureOffsetUniformLocation{ -1 };
	};
}
