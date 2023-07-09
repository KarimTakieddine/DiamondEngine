#include "GraphicsContext.h"
#include "LogManager.h"
#include "SceneLoader.h"

#include "BehaviourRegistry.h"
#include "RotateBehaviourBuilder.h"
#include "RotateBehaviourConfigParser.h"

int main(int argc, char** argv) {

	try {
		std::unique_ptr<diamond_engine::GraphicsContext> graphicsContext = std::make_unique<diamond_engine::GraphicsContext>();
		graphicsContext->InitializeWindow({ 1280, 960 }, "Hello!");
		graphicsContext->InitializeGLEW();

		std::shared_ptr<diamond_engine::SharedShaderStore> sharedShaderStore = std::make_shared<diamond_engine::SharedShaderStore>();
		sharedShaderStore->Load("shaders");

		diamond_engine::RegisterBehaviour("RotateBehaviour", &diamond_engine::RotateBehaviourConfigParser::Parse, &diamond_engine::RotateBehaviourBuilder::Build);

		std::shared_ptr<diamond_engine::SceneLoader> sceneLoader = std::make_shared<diamond_engine::SceneLoader>();
		sceneLoader->SetSharedShaderStore(sharedShaderStore);
		sceneLoader->LoadScenes("scenes");

		diamond_engine::Scene* sampleScene = sceneLoader->FindScene("sample_scene");
		graphicsContext->SetScene(sampleScene);

		graphicsContext->Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}