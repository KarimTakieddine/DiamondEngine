#pragma once

#include "DrawCall.h"
#include "MemoryPoolAllocator.hpp"
#include "MeshType.h"
#include "VertexAttribute.h"

namespace diamond_engine
{
	// What is a uniform really?

	// It's a location and a pointer to some memory (i.e. vec3, mat4 and so on...)
	// It has a type (e.g. float, double)
	// How to handle them efficiently god dang it!!!

	/*
	* We bind the shader
	* We upload camera matrices once
	* for each GameObject:
	*	We upload GameObject matrices (Transforms) once
	*	We upload GameObject material color and texture offset once
	*
	* Ok I see now
	*
	* So the camera matrices, and all the transform matrices these will refer to a contiguous
	* allocator
	*
	* The rest is a draw call for each object, with which we register it like hey renderer
	* I want you to draw this object like this aka draw triangles like this with these points
	*
	* During update:
	*
	* Each GameObject will want to manipulate these matrices in memory, which is fine since they are
	* all very close together. GREAT
	*
	* Then sequentially:
	*
	* We upload each matrix with a descriptor, like here is the memory to copy from and type etc...
	* We have functions to upload each type, which is of course cool
	* We draw using the descriptor that the object was registered with
	*
	* That's it!
	*/

	using RenderComponentList = std::vector<std::unique_ptr<IRenderComponent>>;

	class Camera;
	class GLAllocator;
	class IRenderComponent;
	class Mesh;
	struct VertexState;
	class Renderer
	{
	public:
		// The Renderer has access to one shared shader program and binds it before uploading all the uniforms
		Renderer(MeshType meshType, GLenum drawMode, const std::shared_ptr<ShaderProgram>& shaderProgram);
		const std::shared_ptr<ShaderProgram>& getShaderProgram() const;
		void render(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);
		void uploadMeshData(GLenum drawType, const VertexState* vertexState, const std::vector<VertexAttribute>& vertexAttributes);

		EngineStatus allocateGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);
		EngineStatus bindToShaderProgram(const RenderComponentList& renderComponents);
		EngineStatus releaseGraphicsMemory(const RenderComponentList& renderComponents, const std::unique_ptr<GraphicsMemoryPool>& memoryPool);

	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram{ nullptr };
		Mesh* m_sharedMesh{ nullptr };
		GLenum m_drawMode{ GL_TRIANGLES };
	};
}
