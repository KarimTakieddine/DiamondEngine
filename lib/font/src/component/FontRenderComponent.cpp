#include "FontRenderComponent.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	const char* FontRenderComponent::getName() const
	{
		return "FontRenderComponent";
	}

	EngineStatus FontRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
			return { "FontRenderComponent::bindToShaderProgram failed. Shared ShaderProgram was null", true };

		if (!m_vxOffset)
			return { "No vertex offset memory allocated for FontRenderComponent", true };

		m_vxOffset->memory.location = shaderProgram->GetUniform("vxOffset");

		if (!m_uvOffset)
			return { "No uv offset memory allocated for FontRenderComponent", true };

		m_uvOffset->memory.location = shaderProgram->GetUniform("uvOffset");

		if (!m_fontSize)
			return { "No font size memory allocated for FontRenderComponent", true };

		m_fontSize->memory.location = shaderProgram->GetUniform("fontSize");

		return { };
	}

	EngineStatus FontRenderComponent::requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		m_vxOffset = memoryPool->requestMemory<UniformVec2>({ { { 0.0f, 0.0f }, 1 } });

		if (!m_vxOffset)
			return { "FontRenderComponent failed to allocate vertex offset graphics memory", true };

		m_uvOffset = memoryPool->requestMemory<UniformVec2>({ { { 0.0f, 0.0f }, 1 } });

		if (!m_uvOffset)
			return { "FontRenderComponent failed to allocate uv offset graphics memory", true };

		m_fontSize = memoryPool->requestMemory<UniformVec2>({ { { 0.0f, 0.0f }, 1 } });

		if (!m_fontSize)
			return { "FontRenderComponent failed to allocate font size graphics memory", true };

		return { };
	}

	EngineStatus FontRenderComponent::releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		memoryPool->freeMemory(sizeof(UniformVec2));
		m_fontSize = nullptr;

		memoryPool->freeMemory(sizeof(UniformVec2));
		m_vxOffset = nullptr;

		memoryPool->freeMemory(sizeof(UniformVec2));
		m_uvOffset = nullptr;

		return { };
	}

	EngineStatus FontRenderComponent::onDrawCallAllocated(DrawCall* drawCall)
	{
		return { };
	}

	EngineStatus FontRenderComponent::uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		uploadUniformMemory(m_vxOffset);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		uploadUniformMemory(m_uvOffset);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		uploadUniformMemory(m_fontSize);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		return { };
	}

	EngineStatus FontRenderComponent::initialize(const RenderComponentConfig* config)
	{
		// TODO

		return { };
	}

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

	const glm::vec2& FontRenderComponent::getUVOffset() const
	{
		return m_uvOffset->memory.value;
	}

	void FontRenderComponent::setUVOffset(const glm::vec2& uvOffset)
	{
		if (!m_uvOffset)
			return;

		m_uvOffset->memory.value = uvOffset;
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
}