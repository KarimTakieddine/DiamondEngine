#pragma once

/*
* A render component is in charge of telling us which
* uniforms to upload, and optionally operating on
* a draw call e.g. a mesh renderer or a material
* 
* They would operate on the same draw call together
* e.g. a material will set the texture
* e.g. a mesh renderer will set the element count / draw mode
* 
* So they get called back with onDrawCallRegistered(ptr)
* And they get asked getRenderUploadDescriptors
*/

#include <memory>
#include <vector>

#include "EngineStatus.h"
#include "RenderUpload.h"

namespace diamond_engine
{
	struct RenderDrawCall;
	struct RenderObject;
	class ShaderProgram;
	class IRenderComponent
	{
	public:
		virtual ~IRenderComponent() = default;
		virtual const char* getName() const = 0;
		virtual std::vector<RenderUpload> getUploads() const = 0;
		virtual EngineStatus onDrawCallRegistered(RenderDrawCall* renderDrawCall) = 0;
		virtual EngineStatus onRenderObjectAllocated(RenderObject* renderObject) = 0;
		virtual EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) = 0;
	};
}
