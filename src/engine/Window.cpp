#include <stdexcept>

#include "Debugger.h"
#include "DeltaTimer.h"
#include "EngineMacros.h"
#include "Window.h"
#include "Input.h"

namespace diamond_engine {
	GLFWWindow::GLFWWindow(
		const Size& size,
		const std::string& title) {
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
		glfwSetWindowSizeCallback(m_handle, &GLFWWindow::OnResize);

		glfwMakeContextCurrent(m_handle);
	}

	void GLFWWindow::setResizeHandler(ResizeHandler handler)
	{
		m_resizeHandler = handler;
	}

	void GLFWWindow::setUpdateHandler(UpdateHandler handler)
	{
		m_updateHandler = handler;
	}

	void GLFWWindow::SetSize(const Size& size) {
		if (size == m_currentSize) {
			return;
		}

		glfwSetWindowSize(m_handle, size.width, size.height);
	}

	const Size& GLFWWindow::getCurrentSize() const
	{
		return m_currentSize;
	}

	void GLFWWindow::StartUpdateLoop() {
		DeltaTimer deltaTimer;
		GLfloat deltaTime = 0.0f;

		while (!glfwWindowShouldClose(m_handle)) {
			deltaTimer.Start();

			DEBUG_EXEC(Debugger::getInstance()->handleAllEvents());

			glfwPollEvents();

			input::StateMonitor::GetInstance().MonitorStates(m_handle);

			m_updateHandler(deltaTime);

			glfwSwapBuffers(m_handle);

			deltaTimer.End();

			deltaTime = deltaTimer.GetDeltaTimeSeconds();
		}
	}

	void GLFWWindow::Close() {
		glfwSetWindowShouldClose(m_handle, GL_TRUE);
	}

	GLFWWindow::~GLFWWindow() {
		glfwDestroyWindow(m_handle);
		glfwTerminate();
	}
	
	/* static */ void GLFWWindow::OnResize(GLFWwindow* handle, int width, int height) {
		GLFWWindow* instance = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(handle));

		const Size updatedSize{ width, height };

		instance->m_resizeHandler(updatedSize);

		instance->m_currentSize = updatedSize;
	}
}