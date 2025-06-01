#include "FontRenderComponent.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	const char* FontRenderComponent::getName() const
	{
		return "FontRenderComponent";
	}

	EngineStatus FontRenderComponent::onDrawCallAllocated(DrawCall* drawCall)
	{
		return { };
	}

	EngineStatus FontRenderComponent::uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		uploadUniformMemory(m_ctopLeft);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		uploadUniformMemory(m_fontSize);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		uploadUniformMemory(m_fontScale);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		uploadUniformMemory(m_vxOffset);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		return { };
	}

	EngineStatus FontRenderComponent::initialize(const RenderComponentConfig* config) { return { }; }

	const glm::vec2& FontRenderComponent::getVXOffset() const
	{
		return m_vxOffset->memory.value;
	}

	void FontRenderComponent::setVXOffset(const glm::vec2& vxOffset)
	{
		if (!m_vxOffset)
			return;

		m_vxOffset->memory.value = vxOffset;
	}

	const glm::vec2& FontRenderComponent::getFontSize() const
	{
		return m_fontSize->memory.value;
	}

	void FontRenderComponent::setFontSize(const glm::vec2& fontSize)
	{
		if (!m_fontSize)
			return;

		m_fontSize->memory.value = fontSize;
	}

	const glm::vec2& FontRenderComponent::getFontScale() const
	{
		return m_fontScale->memory.value;
	}

	void FontRenderComponent::setFontScale(const glm::vec2& fontScale)
	{
		if (!m_fontScale)
			return;

		const GLfloat fontScaleX = fontScale.x;
		const GLfloat fontScaleY = fontScale.y;

		const glm::vec2 clampedScale{
			fontScaleX > 1.0f ? 1.0f : ( fontScaleX < 0.0f ? 0.0f : fontScaleX ),
			fontScaleY > 1.0f ? 1.0f : ( fontScaleY < 0.0f ? 0.0f : fontScaleY ),
		};

		m_fontScale->memory.value = clampedScale;
	}

	const glm::vec2& FontRenderComponent::getCTopLeft() const
	{
		return m_ctopLeft->memory.value;
	}

	void FontRenderComponent::setCTopLeft(const glm::vec2& ctopLeft)
	{
		if (!m_ctopLeft)
			return;

		m_ctopLeft->memory.value = ctopLeft;
	}

	RenderDescriptor FontRenderComponent::getRenderDescriptor() const
	{
		return
		{
			{
				{
					"ctopLeft",
					1,
					UniformDescriptor::Type::VECTOR_2
				},
				{
					"fontSize",
					1,
					UniformDescriptor::Type::VECTOR_2
				},
				{
					"fontScale",
					1,
					UniformDescriptor::Type::VECTOR_2
				},
				{
					"vxOffset",
					1,
					UniformDescriptor::Type::VECTOR_2
				}
			}
		};
	}

	void FontRenderComponent::onGraphicsMemoryAllocated(GLubyte* allocatedMemory)
	{
		m_ctopLeft	= reinterpret_cast<UniformVec2*>(allocatedMemory);
		m_fontSize	= reinterpret_cast<UniformVec2*>(allocatedMemory + sizeof(UniformVec2));
		m_fontScale = reinterpret_cast<UniformVec2*>(allocatedMemory + ( 2 * sizeof(UniformVec2) ));
		m_vxOffset	= reinterpret_cast<UniformVec2*>(allocatedMemory + ( 3 * sizeof(UniformVec2) ));

		m_ctopLeft->memory.value	= { 0.0f, 0.0f };
		m_fontSize->memory.value	= { 0.0f, 0.0f };
		m_fontScale->memory.value	= { 0.0f, 0.0f };
		m_vxOffset->memory.value	= { 0.0f, 0.0f };
	}
}