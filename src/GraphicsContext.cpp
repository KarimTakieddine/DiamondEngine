#include <stdexcept>

#include "GraphicsContext.h"

namespace diamond_engine {
	void GraphicsContext::InitializeWindow(const Size& windowSize, const std::string& windowTitle) {
		/*
			Additional calls to this function after successful initialization
			but before termination will return GLFW_TRUE immediately.
		*/

		if (!glfwInit()) {
			const char* errorBuffer = nullptr;
			const int errorCode = glfwGetError(&errorBuffer);
			throw std::runtime_error(
				"Could not initialize GLFW library! Error code: " + std::to_string(errorCode)
				+ " - Message: " + std::string(errorBuffer));
		}

		m_window.reset(new Window(
			windowSize,
			windowTitle,
			std::bind(&GraphicsContext::OnWindowResize, this, std::placeholders::_1),
			std::bind(&GraphicsContext::OnWindowUpdate, this, std::placeholders::_1)));
	}

	void GraphicsContext::InitializeGLEW() {
		glewExperimental = GL_TRUE;

		GLenum status = glewInit();

		if (status != GLEW_OK) {
			std::stringstream errorBuffer;
			errorBuffer << glewGetErrorString(status);
			throw std::runtime_error("GraphicsContext::InitializeGLEW() failed - Message: " + errorBuffer.str());
		}
	}

	void GraphicsContext::SetScene(std::unique_ptr<Scene> scene) {
		m_scene = std::move(scene);
	}

	void GraphicsContext::Execute() {
		if (!m_window) {
			throw std::runtime_error("Cannot Execute() GraphicsContext with no window");
		}

		m_window->StartUpdateLoop();
	}

	GraphicsContext::~GraphicsContext() {
		m_scene.reset(nullptr);
		m_window.reset(nullptr);

		glfwTerminate();
	}

	void GraphicsContext::OnWindowResize(const Size& windowSize) {
		// TODO

		glViewport(0, 0, windowSize.width, windowSize.height);
	}

	void GraphicsContext::OnWindowUpdate(GLfloat deltaTime) {
		if (m_scene) {
			m_scene->Update();
			m_scene->Render();
		}
	}
}