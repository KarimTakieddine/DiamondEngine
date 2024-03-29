#pragma once

#include <memory>

#include "IRenderComponent.h"

namespace diamond_engine
{
	class Mesh;
	class GLAllocator;
	class MeshRenderComponent : public IRenderComponent
	{
	public:
		~MeshRenderComponent() final override = default;
		std::vector<RenderUpload> getUploads() const final override;
		EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) final override;
		EngineStatus onRenderObjectAllocated(RenderObject* renderObject) final override;
		EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) final override;
		
		void setSharedBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator);
		void setSharedMesh(const std::shared_ptr<Mesh>& sharedMesh);
		void setDrawMode(GLenum drawMode);
		GLenum getDrawMode() const;

	private:
		std::shared_ptr<Mesh> m_sharedMesh{ nullptr };
		std::shared_ptr<GLAllocator> m_sharedBufferAllocator{ nullptr };
		RenderDrawCall* m_drawCall{ nullptr };
		GLenum m_drawMode{ GL_STATIC_DRAW };
		GLuint m_vertexBufferObject{ 0 };
		GLuint m_elementBufferObject{ 0 };
		GLint m_vertexAttributeLocation{ -1 };
		GLint m_colorAttributeLocation{ -1 };
		GLint m_textureCoordAttributeLocation{ -1 };
	};
}
