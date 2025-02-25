#pragma once

#include <functional>
#include <string>

#include <glfw/glfw3.h>

#include "Size.h"

namespace diamond_engine {
class GLFWWindow {
public:
	using ResizeHandler = std::function<void(const Size&)>;
	using UpdateHandler = std::function<void(GLfloat)>;

	static const int kOpenGLVersionMajor = 4;
	static const int kOpenGLVersionMinor = 6;

	static const size_t kMaxErrorBufferSize = 2 << 7;

	GLFWWindow(
		const Size& size,
		const std::string& title);

	void setResizeHandler(ResizeHandler handler);

	void setUpdateHandler(UpdateHandler handler);

	GLFWwindow* GetHandle() const { return m_handle; }

	operator bool() const { return m_handle; }

	void SetSize(const Size& size);

	const Size& getCurrentSize() const;

	void StartUpdateLoop();

	void Close();

	~GLFWWindow();

private:
	static void OnResize(GLFWwindow* handle, int width, int height);

	Size m_currentSize				{ };
	ResizeHandler m_resizeHandler	{ nullptr };
	UpdateHandler m_updateHandler	{ nullptr };
	GLFWwindow* m_handle			{ nullptr };
};
}
