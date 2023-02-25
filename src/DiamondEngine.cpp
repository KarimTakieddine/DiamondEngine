#include "GraphicsContext.h"
#include "LogManager.h"

int main(int argc, char** argv) {

	try {
		diamond_engine::GraphicsContext graphicsContext;
		graphicsContext.InitializeWindow({ 640, 480 }, "Hello!");
		graphicsContext.InitializeGLEW();
		graphicsContext.Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());
	}

	return 0;
}