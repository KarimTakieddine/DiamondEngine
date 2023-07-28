#pragma once

#include <functional>
#include <string>

#include <glfw/glfw3.h>

#include "Size.h"

namespace diamond_engine {
class Window {
public:
	using ResizeHandler = std::function<void(const Size&)>;
	using UpdateHandler = std::function<void(GLfloat)>;

	static const int kOpenGLVersionMajor = 4;
	static const int kOpenGLVersionMinor = 6;

	static const size_t kMaxErrorBufferSize = 2 << 7;

	Window(
		const Size& size,
		const std::string& title,
		ResizeHandler resizeHandler,
		UpdateHandler updateHandler);

	GLFWwindow* GetHandle() const { return m_handle; }

	operator bool() const { return m_handle; }

	void SetSize(const Size& size);

	void StartUpdateLoop();

	void Close();

	~Window();

private:
	static void OnResize(GLFWwindow* handle, int width, int height);

	Size m_currentSize				{ };
	ResizeHandler m_resizeHandler	{ nullptr };
	UpdateHandler m_updateHandler	{ nullptr };
	GLFWwindow* m_handle			{ nullptr };
};
}
