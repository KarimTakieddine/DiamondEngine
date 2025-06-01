#pragma once

#include "DrawCall.h"
#include "MemoryPoolAllocator.hpp"
#include "MeshType.h"
#include "VertexAttribute.h"

namespace diamond_engine
{
	using RenderComponentList = std::vector<std::unique_ptr<IRenderComponent>>;

	class Camera;
	class GLAllocator;
	class IRenderComponent;
	class Mesh;
	struct VertexState;
	class Renderer
	{
	public:
		Renderer(GLuint vertexArrayObject, MeshType meshType, GLenum drawMode, const std::shared_ptr<ShaderProgram>& shaderProgram);
		const std::shared_ptr<ShaderProgram>& getShaderProgram() const;
		GLuint getVertexArrayObject() const;
		void render(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);
		void uploadMeshData(GLenum drawType, const VertexState* vertexState, const std::vector<VertexAttribute>& vertexAttributes);

		EngineStatus allocateGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);
		EngineStatus releaseGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);

	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram{ nullptr };
		Mesh* m_sharedMesh{ nullptr };
		GLenum m_vertexArrayObject{ 0 };
		GLenum m_drawMode{ GL_TRIANGLES };
	};
}
