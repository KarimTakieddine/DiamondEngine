#pragma once

#include <memory>

#include "AlignedAllocator.hpp"
#include "EngineStatus.h"
#include "GameInstance.h"
#include "GameSceneConfig.h"
#include "RenderObject.h"

namespace diamond_engine
{
	class GLAllocator;
	class RenderingSubsystem;
	class TextureLoader;
	class GameInstanceManager
	{
	public:
		GameInstanceManager();

		void setRenderingSubsystem(const std::shared_ptr<RenderingSubsystem>& renderingSubsystem);
		void unloadCurrentScene();
		EngineStatus loadScene(const GameSceneConfig& sceneConfig);
		std::unique_ptr<GameInstance> createInstance();
		EngineStatus registerInstance(std::unique_ptr<GameInstance> instance, const GameInstanceConfig* instanceConfig);
		void updateInstances(GLfloat deltaTime);

	private:
		std::vector<std::unique_ptr<GameInstance>> m_instances;
		std::shared_ptr<RenderingSubsystem> m_renderingSubsystem{ nullptr };
		std::unique_ptr<AlignedAllocator<RenderObject, 4>> m_renderObjectAllocator{ nullptr };
	};
}
