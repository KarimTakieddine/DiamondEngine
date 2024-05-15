#include "DrawCall.h"
#include "RenderDrawCall.h"
#include "RenderMaterial.h"
#include "MaterialComponentConfig.h"
#include "MaterialRenderComponent.h"
#include "RenderObject.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	std::vector<RenderUpload> MaterialRenderComponent::getUploads() const
	{
		return {
			/*{ &m_material->color,			RenderUploadType::Vector3, m_colorUniformLocation },
			{ &m_material->textureOffset,	RenderUploadType::Vector2, m_textureOffsetUniformLocation }*/
		};
	}

	EngineStatus MaterialRenderComponent::onDrawCallRegistered(RenderDrawCall* renderDrawCall)
	{
		/*if (!renderDrawCall)
		{
			return { "MaterialRenderComponent::onDrawCallRegistered failed. RendererDrawCall was null", true };
		}

		m_renderDrawCall = renderDrawCall;
		
		setTexture(m_texture);
		setColor(m_color);
		setTextureOffset(m_textureOffset);*/

		return { };
	}

	EngineStatus MaterialRenderComponent::onDrawCallAllocated(DrawCall* drawCall)
	{
		if (!drawCall)
		{
			return { "MaterialRenderComponent::formatDrawCall failed. DrawCall was null", true };
		}

		m_drawCall = drawCall;

		//m_renderDrawCall = renderDrawCall;

		//setTexture(m_texture);
		//setColor(m_color);
		//setTextureOffset(m_textureOffset);

		return { };
	}

	EngineStatus MaterialRenderComponent::uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		if (!memoryPool)
		{
			return { "MaterialRenderComponent::uploadGraphicsMemory failed. Graphics memory pool was NULL", true };
		}

		// TODO: Upload color
		uploadUniformMemory(m_colorMemory);
		memoryPool->advanceSeek(sizeof(UniformVec3));
		
		// TODO: Upload texture offset
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

		//m_colorUniformLocation			= shaderProgram->GetUniform("materialColor");
		//m_textureOffsetUniformLocation	= shaderProgram->GetUniform("materialTextureOffset");

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
		/*if (m_renderDrawCall)
		{
			m_renderDrawCall->texture = texture.index;
		}*/

		if (m_drawCall)
		{
			m_drawCall->texture = texture.index;
		}

		// m_texture = texture;
	}

	void MaterialRenderComponent::setColor(const glm::vec3& color)
	{
		/*if (m_material)
		{
			m_material->color = color;
		}*/

		if (m_colorMemory)
		{
			m_colorMemory->memory.value = color;
		}

		//m_color = color;
	}

	/*const glm::vec3& MaterialRenderComponent::getColor() const
	{
		return m_colorMemory->memory.value;
	}

	glm::vec3& MaterialRenderComponent::getColor()
	{
		return m_color;
	}*/

	void MaterialRenderComponent::setTextureOffset(const glm::vec2& textureOffset)
	{
		/*if (m_material)
		{
			m_material->textureOffset = textureOffset;
		}*/

		//m_textureOffset = textureOffset;

		if (m_textureOffsetMemory)
		{
			m_textureOffsetMemory->memory.value = textureOffset;
		}
	}

	/*const glm::vec2& MaterialRenderComponent::getTextureOffset() const
	{
		return m_textureOffset;
	}

	glm::vec2& MaterialRenderComponent::getTextureOffset()
	{
		return m_textureOffset;
	}*/

	EngineStatus MaterialRenderComponent::requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool)
	{
		m_colorMemory = memoryPool->requestMemory<UniformVec3>({ { {1.0f, 1.0f, 1.0f }, 1 } });
		
		if (!m_colorMemory)
		{
			return { "Failed to allocate color graphics memory for MaterialRenderComponent", true };
		}
		
		m_textureOffsetMemory = memoryPool->requestMemory<UniformVec2>({ { {1.0f, 1.0f }, 1 } });
	
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