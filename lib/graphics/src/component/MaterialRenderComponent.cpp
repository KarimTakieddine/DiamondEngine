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
			return { "MaterialRenderComponent::onDrawCallAllocated failed. DrawCall was null", true };
		}

		m_drawCall = drawCall;

		return { };
	}

	void MaterialRenderComponent::setTexture(GLuint texture)
	{
		if (m_drawCall)
		{
			m_drawCall->texture = texture;
		}
	}

	void MaterialRenderComponent::setColor(const glm::vec3& color)
	{
		if (m_colorMemory)
		{
			m_colorMemory->memory.value = color;
		}
	}

	const glm::vec3& MaterialRenderComponent::getColor() const
	{
		return m_colorMemory->memory.value;
	}

	void MaterialRenderComponent::setTextureOffset(const glm::vec2& textureOffset)
	{
		if (m_textureOffsetMemory)
		{
			m_textureOffsetMemory->memory.value = textureOffset;
		}
	}

	const glm::vec2& MaterialRenderComponent::getTextureOffset() const
	{
		return m_textureOffsetMemory->memory.value;
	}

	glm::vec2& MaterialRenderComponent::getTextureOffset()
	{
		return m_textureOffsetMemory->memory.value;
	}

	EngineStatus MaterialRenderComponent::initialize(const RenderComponentConfig* config)
	{
		const MaterialComponentConfig* materialConfig = dynamic_cast<const MaterialComponentConfig*>(config);

		if (!materialConfig)
			return { "MaterialRenderComponent::initialize failed. Could not convert config parameter to target type", true };

		setTexture(TextureLoader::getInstance()->GetTexture(materialConfig->getTextureName()).index);
		setColor(materialConfig->getColor());
		setTextureOffset(materialConfig->getTextureOffset());

		return { };
	}

	RenderDescriptor MaterialRenderComponent::getRenderDescriptor() const
	{
		return
		{
			{
				{
					"materialColor",
					1,
					UniformDescriptor::Type::VECTOR_3
				},
				{
					"materialTextureOffset",
					1,
					UniformDescriptor::Type::VECTOR_2
				}
			}
		};
	}

	void MaterialRenderComponent::onGraphicsMemoryAllocated(GLubyte* allocatedMemory)
	{
		m_colorMemory			= reinterpret_cast<UniformVec3*>(allocatedMemory);
		m_textureOffsetMemory	= reinterpret_cast<UniformVec2*>(allocatedMemory + sizeof(UniformVec3));

		m_colorMemory->memory.value			= { 1.0f, 1.0f, 1.0f };
		m_textureOffsetMemory->memory.value = { 0.0f, 0.0f };
	}
}