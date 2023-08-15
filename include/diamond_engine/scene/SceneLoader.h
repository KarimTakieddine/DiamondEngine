#pragma once

#include <filesystem>
#include <unordered_map>

#include "Scene.h"

namespace diamond_engine {
class SceneLoader {
public:
	using SceneMap = std::unordered_map<std::string, std::unique_ptr<Scene>>;

	void SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

	void SetSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader);

	void LoadScenes(const std::filesystem::path& rootDirectory);

	Scene* FindScene(const std::string& name) const;

private:
	SceneMap m_sceneMap{ };
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore{ nullptr };
	std::shared_ptr<TextureLoader> m_sharedTextureLoader{ nullptr };
};
}
