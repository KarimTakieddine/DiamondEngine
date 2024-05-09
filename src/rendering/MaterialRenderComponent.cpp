#include "RenderDrawCall.h"
#include "RenderMaterial.h"
#include "MaterialRenderComponent.h"
#include "RenderObject.h"
#include "ShaderProgram.h"

namespace diamond_engine
{
	std::vector<RenderUpload> MaterialRenderComponent::getUploads() const
	{
		return {
			{ &m_material->color,			RenderUploadType::Vector3, m_colorUniformLocation },
			{ &m_material->textureOffset,	RenderUploadType::Vector2, m_textureOffsetUniformLocation }
		};
	}

	EngineStatus MaterialRenderComponent::onDrawCallRegistered(RenderDrawCall* renderDrawCall)
	{
		if (!renderDrawCall)
		{
			return { "MaterialRenderComponent::onDrawCallRegistered failed. RendererDrawCall was null", true };
		}

		m_renderDrawCall = renderDrawCall;
		
		setTexture(m_texture);
		setColor(m_color);
		setTextureOffset(m_textureOffset);

		return { };
	}

	EngineStatus MaterialRenderComponent::onRenderObjectAllocated(RenderObject* renderObject)
	{
		if (!renderObject)
		{
			return { "MaterialRenderComponent::onRenderObjectAllocated failed. RenderObject was null", true };
		}

		m_material = &renderObject->material;

		return { };
	}

	EngineStatus MaterialRenderComponent::bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		if (!shaderProgram)
		{
			return { "MaterialRenderComponent::bindToShaderProgram failed. Shared ShaderProgram was null", true };
		}

		m_colorUniformLocation			= shaderProgram->GetUniform("materialColor");
		m_textureOffsetUniformLocation	= shaderProgram->GetUniform("materialTextureOffset");

		return { };
	}

	void MaterialRenderComponent::setTexture(const Texture& texture)
	{
		if (m_renderDrawCall)
		{
			m_renderDrawCall->texture = texture.index;
		}

		m_texture = texture;
	}

	const Texture& MaterialRenderComponent::getTexture() const
	{
		return m_texture;
	}

	Texture& MaterialRenderComponent::getTexture()
	{
		return m_texture;
	}

	void MaterialRenderComponent::setColor(const glm::vec3& color)
	{
		if (m_material)
		{
			m_material->color = color;
		}

		m_color = color;
	}

	const glm::vec3& MaterialRenderComponent::getColor() const
	{
		return m_color;
	}

	glm::vec3& MaterialRenderComponent::getColor()
	{
		return m_color;
	}

	void MaterialRenderComponent::setTextureOffset(const glm::vec2& textureOffset)
	{
		if (m_material)
		{
			m_material->textureOffset = textureOffset;
		}

		m_textureOffset = textureOffset;
	}

	const glm::vec2& MaterialRenderComponent::getTextureOffset() const
	{
		return m_textureOffset;
	}

	glm::vec2& MaterialRenderComponent::getTextureOffset()
	{
		return m_textureOffset;
	}
}