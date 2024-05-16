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
#include "MemoryPoolAllocator.hpp"

namespace diamond_engine
{
	struct DrawCall;
	class ShaderProgram;
	class RenderComponentConfig;
	class IRenderComponent
	{
	public:
		virtual ~IRenderComponent() = default;
		virtual const char* getName() const = 0;
		virtual EngineStatus bindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) = 0;
		virtual EngineStatus requestGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) = 0;
		virtual EngineStatus releaseGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) = 0;
		virtual EngineStatus onDrawCallAllocated(DrawCall* drawCall) = 0;
		virtual EngineStatus uploadGraphicsMemory(const std::unique_ptr<GraphicsMemoryPool>& memoryPool) = 0;
		virtual EngineStatus initialize(const RenderComponentConfig* config) = 0;
	};
}
