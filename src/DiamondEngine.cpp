#include "GraphicsContext.h"
#include "GLAllocator.h"
#include "LogManager.h"

int main(int argc, char** argv) {

	try {
		diamond_engine::GraphicsContext graphicsContext;
		graphicsContext.InitializeWindow({ 1280, 960 }, "Hello!");
		graphicsContext.InitializeGLEW();

		diamond_engine::GLAllocator glBufferAllocator(glGenBuffers, glDeleteBuffers);
		glBufferAllocator.Reserve(12);
		glBufferAllocator.Allocate(6);
		LOG_INFO(glBufferAllocator.ToString());
		glBufferAllocator.Allocate(6);
		LOG_INFO(glBufferAllocator.ToString());
		glBufferAllocator.Free(9);
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_1 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_1));
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_2 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_2));
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_3 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_3));
		LOG_INFO(glBufferAllocator.ToString());

		glBufferAllocator.Allocate(1);

		GLuint buffer_4 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_4));
		LOG_INFO(glBufferAllocator.ToString());

		graphicsContext.Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}