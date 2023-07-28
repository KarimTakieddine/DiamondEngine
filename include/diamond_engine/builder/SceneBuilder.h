#pragma once

#include "Scene.h"

namespace diamond_engine {
class SceneConfig;

class SceneBuilder {
public:
	void SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator);

	void SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

	std::unique_ptr<Scene> Build(const SceneConfig* sceneConfig);

private:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore{ nullptr };
	std::shared_ptr<GLAllocator> m_bufferAllocator{ nullptr };
};
}
