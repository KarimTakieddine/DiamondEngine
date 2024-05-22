#pragma once

#include "IRenderComponent.h"
#include "UniformMemory.hpp"

namespace diamond_engine
{
	class FontRenderComponent : public IRenderComponent
	{
	public:
		~FontRenderComponent() final override = default;
		
		const char* getName() const final override;
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;
		EngineStatus requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus onDrawCallAllocated(DrawCall* drawCall) final override;
		EngineStatus uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) final override;
		EngineStatus initialize(const RenderComponentConfig* config) final override;

		const glm::vec2& getVXOffset() const;
		void setVXOffset(const glm::vec2& vxOffset);

		const glm::vec2& getUVOffset() const;
		void setUVOffset(const glm::vec2& uvOffset);

		const glm::vec2& getFontSize() const;
		void setFontSize(const glm::vec2& fontSize);

		const glm::vec2& getCTopLeft() const;
		void setCTopLeft(const glm::vec2& ctopLeft);

	private:
		UniformVec2* m_ctopLeft{ nullptr };
		UniformVec2* m_fontSize{ nullptr };
		UniformVec2* m_vxOffset{ nullptr };
		UniformVec2* m_uvOffset{ nullptr };
	};
}
