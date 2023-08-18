#include <stdexcept>

#include "DeltaTimer.h"
#include "Window.h"
#include "Input.h"

namespace diamond_engine {
	Window::Window(
		const Size& size,
		const std::string& title,
		ResizeHandler resizeHandler,
		UpdateHandler updateHandler) {
		if (!resizeHandler) {
			throw std::runtime_error("Window creation requires ResizeHandler");
		}

		m_resizeHandler = resizeHandler;

		if (!updateHandler) {
			throw std::runtime_error("Window creation requires UpdateHandler");
		}

		m_updateHandler = updateHandler;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_handle = glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, nullptr);

		if (!m_handle) {
			const char* errorBuffer = nullptr;
			const int errorCode = glfwGetError(&errorBuffer);
			throw std::runtime_error(
				"Could not initialize Window! Error code: " + std::to_string(errorCode)
				+ " - Message: " + std::string(errorBuffer));
		}

		m_currentSize = size;

		glfwSetWindowUserPointer(m_handle, this);
		glfwSetWindowSizeCallback(m_handle, &Window::OnResize);

		glfwMakeContextCurrent(m_handle);
	}

	void Window::SetSize(const Size& size) {
		if (size == m_currentSize) {
			return;
		}

		glfwSetWindowSize(m_handle, size.width, size.height);
	}

	void Window::StartUpdateLoop() {
		DeltaTimer deltaTimer;
		GLfloat deltaTime = 0.0f;

		while (!glfwWindowShouldClose(m_handle)) {
			deltaTimer.Start();

			glfwPollEvents();

			input::StateMonitor::GetInstance().MonitorStates(m_handle);

			m_updateHandler(deltaTime);

			glfwSwapBuffers(m_handle);

			deltaTimer.End();

			deltaTime = deltaTimer.GetDeltaTimeSeconds();
		}
	}

	void Window::Close() {
		glfwSetWindowShouldClose(m_handle, GL_TRUE);
	}

	Window::~Window() {
		glfwDestroyWindow(m_handle);
	}
	
	/* static */ void Window::OnResize(GLFWwindow* handle, int width, int height) {
		Window* instance = reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));

		const Size updatedSize{ width, height };

		instance->m_resizeHandler(updatedSize);

		instance->m_currentSize = updatedSize;
	}
}