#pragma once

#include "AlignedAllocator.hpp"
#include "RenderInstance.h"

namespace diamond_engine
{
	class GLAllocator;
	class RenderingSubsystem;
	class SharedShaderStore;

	using SharedRenderObjectAllocator	= std::shared_ptr<AlignedAllocator<RenderObject, 4>>;
	using SharedBufferAllocator			= std::shared_ptr<GLAllocator>;
	using SharedRenderingSubsystem		= std::shared_ptr<RenderingSubsystem>;

	class Camera;
	class GameInstanceConfig;
	class RenderInstanceManager
	{
	public:
		EngineStatus setSharedObjectAllocator(const SharedRenderObjectAllocator& sharedObjectAllocator);
		EngineStatus setSharedRenderingSubsystem(const SharedRenderingSubsystem& sharedRenderingSubsystem);
		EngineStatus setShaderProgramName(const std::string& shaderProgramName);

		void setSharedBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator);
		EngineStatus allocateInstance(const GameInstanceConfig* instanceConfig);
		void setCamera(const std::shared_ptr<Camera>& camera);

	protected:
		virtual std::vector<std::unique_ptr<IRenderComponent>> getComponents(const GameInstanceConfig* instanceConfig) const = 0;
		SharedBufferAllocator m_bufferAllocator{ nullptr };

	private:
		std::vector<std::unique_ptr<RenderInstance>> m_instances;
		SharedRenderObjectAllocator m_objectAllocator{ nullptr };
		SharedRenderingSubsystem m_renderingSubsystem{ nullptr };
		std::shared_ptr<Camera> m_camera{ nullptr };
		std::string m_shaderProgramName{ "unnamed" };
	};
}
