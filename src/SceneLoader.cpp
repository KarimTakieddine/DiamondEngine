#include <stdexcept>

#include <pugixml.hpp>

#include "SceneBuilder.h"
#include "SceneConfigParser.h"
#include "SceneLoader.h"

namespace diamond_engine {
	void SceneLoader::SetSharedShaderStore(const std::shared_ptr<SharedShaderStore>& sharedShaderStore) {
		m_sharedShaderStore = sharedShaderStore;
	}

	void SceneLoader::LoadScenes(const std::filesystem::path& rootDirectory) {
		m_sceneMap.clear();

		if (!std::filesystem::is_directory(rootDirectory)) {
			throw std::runtime_error("Root SceneLoader directory: " + rootDirectory.string() + " is invalid");
		}

		for (const auto& sceneFile : std::filesystem::directory_iterator(rootDirectory)) {
			if (!std::filesystem::is_regular_file(sceneFile)) {
				continue;
			}

			const std::filesystem::path& scenePath = sceneFile.path();

			const std::string sceneName = scenePath.stem().string();

			pugi::xml_document sceneDocument;
			pugi::xml_parse_result sceneParseResult = sceneDocument.load_file(scenePath.c_str(), pugi::parse_default, pugi::xml_encoding::encoding_utf8);

			if (!sceneParseResult) {
				throw std::runtime_error("Failed to parse <Scene/> XML document: " + std::string(sceneParseResult.description()));
			}

			SceneConfigParser sceneConfigParser;
			std::unique_ptr<SceneConfig> sceneConfig = sceneConfigParser.Parse(sceneDocument);

			SceneBuilder sceneBuilder;

			if (!m_sharedShaderStore) {
				throw std::runtime_error("No shared shader store set for SceneLoader instance");
			}
			sceneBuilder.SetSharedShaderStore(m_sharedShaderStore);
			std::unique_ptr<Scene> scene = sceneBuilder.Build(sceneConfig.get());

			m_sceneMap[sceneName] = std::move(scene);
		}
	}

	Scene* SceneLoader::FindScene(const std::string& name) const {
		auto sceneIt = m_sceneMap.find(name);

		if (sceneIt == m_sceneMap.cend()) {
			return nullptr;
		}

		return sceneIt->second.get();
	}
}