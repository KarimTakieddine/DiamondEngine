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

		m_drawCall			= renderDrawCall;
		m_drawCall->texture = m_texture.index;

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
}