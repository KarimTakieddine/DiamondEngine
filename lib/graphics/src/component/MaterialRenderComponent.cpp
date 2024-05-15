#include "DrawCall.h"
#include "MaterialComponentConfig.h"
#include "MaterialRenderComponent.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	EngineStatus MaterialRenderComponent::onDrawCallAllocated(DrawCall* drawCall)
	{
		if (!drawCall)
		{
			return { "MaterialRenderComponent::formatDrawCall failed. DrawCall was null", true };
		}

		m_drawCall = drawCall;

		return { };
	}

	EngineStatus MaterialRenderComponent::uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		if (!memoryPool)
		{
			return { "MaterialRenderComponent::uploadGraphicsMemory failed. Graphics memory pool was NULL", true };
		}

		uploadUniformMemory(m_colorMemory);
		memoryPool->advanceSeek(sizeof(UniformVec3));

		uploadUniformMemory(m_textureOffsetMemory);
		memoryPool->advanceSeek(sizeof(UniformVec2));

		return { };
	}

	EngineStatus MaterialRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
		{
			return { "MaterialRenderComponent::bindToShaderProgram failed. Shared ShaderProgram was null", true };
		}

		if (!m_colorMemory)
		{
			return { "Failed to allocate color graphics memory for MaterialRenderComponent", true };
		}
		
		m_colorMemory->memory.location = shaderProgram->GetUniform("materialColor");
	
		if (!m_textureOffsetMemory)
		{
			return { "Failed to allocate texture offset graphics memory for MaterialRenderComponent", true };
		}

		m_textureOffsetMemory->memory.location = shaderProgram->GetUniform("materialTextureOffset");

		return { };
	}

	void MaterialRenderComponent::setTexture(const Texture& texture)
	{
		if (m_drawCall)
		{
			m_drawCall->texture = texture.index;
		}
	}

	void MaterialRenderComponent::setColor(const glm::vec3& color)
	{
		if (m_colorMemory)
		{
			m_colorMemory->memory.value = color;
		}
	}

	void MaterialRenderComponent::setTextureOffset(const glm::vec2& textureOffset)
	{
		if (m_textureOffsetMemory)
		{
			m_textureOffsetMemory->memory.value = textureOffset;
		}
	}

	EngineStatus MaterialRenderComponent::requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		m_colorMemory = memoryPool->requestMemory<UniformVec3>({ { {1.0f, 1.0f, 1.0f }, 1 } });
		
		if (!m_colorMemory)
		{
			return { "Failed to allocate color graphics memory for MaterialRenderComponent", true };
		}
		
		m_textureOffsetMemory = memoryPool->requestMemory<UniformVec2>({ { { 0.0f, 0.0f }, 1 } });
	
		if (!m_textureOffsetMemory)
		{
			return { "Failed to allocate texture offset graphics memory for MaterialRenderComponent", true };
		}

		return { };
	}

	EngineStatus MaterialRenderComponent::releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		// TODO: Error handling

		memoryPool->freeMemory(sizeof(UniformVec2));
		m_textureOffsetMemory = nullptr;

		memoryPool->freeMemory(sizeof(UniformVec3));
		m_colorMemory = nullptr;

		return { };
	}

	EngineStatus MaterialRenderComponent::initialize(const RenderComponentConfig* config)
	{
		const MaterialComponentConfig* materialConfig = dynamic_cast<const MaterialComponentConfig*>(config);

		if (!materialConfig)
			return { "MaterialRenderComponent::initialize failed. Could not convert config parameter to target type", true };

		setTexture(TextureLoader::getInstance()->GetTexture(materialConfig->getTextureName()));
		setColor(materialConfig->getColor());
		setTextureOffset(materialConfig->getTextureOffset());

		return { };
	}
}