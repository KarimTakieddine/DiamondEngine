#include "GraphicsContext.h"
#include "LogManager.h"
#include "SharedShaderStore.h"

int main(int argc, char** argv) {

	try {
		std::unique_ptr<diamond_engine::GraphicsContext> graphicsContext = std::make_unique<diamond_engine::GraphicsContext>();
		graphicsContext->InitializeWindow({ 1280, 960 }, "Hello!");
		graphicsContext->InitializeGLEW();

		std::shared_ptr<diamond_engine::SharedShaderStore> sharedShaderStore = std::make_shared<diamond_engine::SharedShaderStore>();
		sharedShaderStore->Load("shaders");

		graphicsContext->Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}