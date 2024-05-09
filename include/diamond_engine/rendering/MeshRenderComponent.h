#pragma once

#include <memory>

#include "IRenderComponent.h"
#include "Mesh.h"

namespace diamond_engine
{
	class GLAllocator;
	class MeshRenderComponent : public IRenderComponent
	{
	public:
		~MeshRenderComponent() final override = default;

		RenderComponentType getComponentType() const final override
		{
			return RenderComponentType::MESH;
		}

		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override;
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;

		void setMesh(Mesh* mesh);
		void setDrawMode(GLenum drawMode);
		GLenum getDrawMode() const;

	private:
		Mesh* m_mesh{ nullptr };
		RenderDrawCall* m_renderDrawCall{ nullptr };
		GLenum m_drawMode{ GL_STATIC_DRAW };
		GLint m_vertexAttributeLocation{ -1 };
		GLint m_colorAttributeLocation{ -1 };
		GLint m_textureCoordAttributeLocation{ -1 };
	};
}
