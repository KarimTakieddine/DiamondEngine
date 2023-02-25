#include <GL/glew.h>

#include "Window.h"
#include "LogManager.h"

void OnResize(const diamond_engine::Size& size) {
	LOG_INFO(diamond_engine::SizeToString(size));
}

void OnUpdate(float deltaTime) {
	LOG_INFO("Delta time: " + std::to_string(deltaTime));
}

int main(int argc, char** argv) {

	try {
		glfwInit();

		diamond_engine::Window window({ 640, 480 }, "Hello!", &OnResize, &OnUpdate);

		if (!window) {
			return 1;
		}

		glfwMakeContextCurrent(window.GetHandle());

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			return 2;
		}

		window.StartUpdateLoop();

		glfwTerminate();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));

	return 0;
}