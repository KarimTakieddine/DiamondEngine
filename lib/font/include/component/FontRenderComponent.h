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
		EngineStatus onDrawCallAllocated(DrawCall* drawCall) final override;
		EngineStatus initialize(const RenderComponentConfig* config) final override;

		const glm::vec2& getVXOffset() const;
		void setVXOffset(const glm::vec2& vxOffset);

		const glm::vec2& getFontSize() const;
		void setFontSize(const glm::vec2& fontSize);

		const glm::vec2& getFontScale() const;
		void setFontScale(const glm::vec2& fontScale);

		const glm::vec2& getCTopLeft() const;
		void setCTopLeft(const glm::vec2& ctopLeft);

		RenderDescriptor getRenderDescriptor() const final override;
		void onGraphicsMemoryAllocated(GLubyte* allocatedMemory) final override;

	private:
		UniformVec2* m_ctopLeft{ nullptr };
		UniformVec2* m_fontSize{ nullptr };
		UniformVec2* m_fontScale{ nullptr };
		UniformVec2* m_vxOffset{ nullptr };
	};
}
