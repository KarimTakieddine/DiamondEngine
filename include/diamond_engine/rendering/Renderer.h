#pragma once

#include "MeshType.h"
#include "RenderDrawCall.h"
#include "RenderUploadObject.h"
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

	struct RenderInstruction
	{
		RenderUploadObject uploadObject;
		RenderDrawCall drawCall;
	};

	class Camera;
	class GLAllocator;
	class IRenderComponent;
	class Mesh;
	class Renderer
	{
	public:
		// The Renderer has access to one shared shader program and binds it before uploading all the uniforms
		Renderer(GLuint vertexArrayObject, MeshType meshType, GLenum drawMode, const std::shared_ptr<ShaderProgram>& shaderProgram);
		void clearRenderInstructions();
		void registerRenderInstruction(const std::vector<std::unique_ptr<IRenderComponent>>& renderComponents, RenderDrawCall* renderDrawCall);
		const std::vector<RenderInstruction>& getInstructions() const;
		std::vector<RenderInstruction>& getInstructions();
		const std::shared_ptr<ShaderProgram>& getShaderProgram() const;
		void setCamera(const std::shared_ptr<Camera>& camera);
		void render();
		void uploadMeshData(const std::vector<VertexAttribute>& vertexAttributes, GLenum drawType);

	private:
		static void performUpload(const RenderUpload& renderUpload);

		std::vector<RenderUpload> m_cameraUploads;
		std::vector<RenderInstruction> m_renderInstructions;
		std::shared_ptr<ShaderProgram> m_shaderProgram{ nullptr };
		std::shared_ptr<Camera> m_camera{ nullptr };
		Mesh* m_sharedMesh{ nullptr };
		GLuint m_vertexArrayObject{ 0 };
		GLenum m_drawMode{ GL_TRIANGLES };
	};
}
