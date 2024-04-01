#include <stdexcept>

#include "GraphicsContext.h"
#include "Input.h"

namespace diamond_engine {
	void GraphicsContext::InitializeWindow(const WindowConfig& windowConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler) {
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
			windowConfig.GetSize(),
			windowConfig.GetTitle(),
			resizeHandler,
			updateHandler));
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

	void GraphicsContext::Initialize(const EngineConfig& engineConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler) {
		InitializeWindow(engineConfig.GetWindowConfig(), updateHandler, resizeHandler);
		InitializeGLEW();
	}

	void GraphicsContext::Execute() {
		if (!m_window) {
			throw std::runtime_error("Cannot Execute() GraphicsContext with no window");
		}

		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		m_window->StartUpdateLoop();
	}

	GLFWwindow* GraphicsContext::getWindowHandle() const
	{
		return m_window ? m_window->GetHandle() : nullptr;
	}

	GraphicsContext::~GraphicsContext() {
		m_window.reset(nullptr);

		glfwTerminate();
	}

	void GraphicsContext::OnWindowResize(const Size& windowSize) {
		glViewport(0, 0, windowSize.width, windowSize.height);
	}
}