#pragma once

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

		// TODO: Set color and stuff...

	private:
		Texture m_texture;
		RenderMaterial* m_material{ nullptr };
		RenderDrawCall* m_drawCall{ nullptr };
		GLint m_colorUniformLocation{ -1 };
		GLint m_textureOffsetUniformLocation{ -1 };
	};
}
