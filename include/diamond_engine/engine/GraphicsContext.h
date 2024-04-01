#pragma once

#include <memory>

#include <GL/glew.h>

#include "EngineConfig.h"
#include "Scene.h"
#include "Window.h"

namespace diamond_engine {
class GraphicsContext {
public:
	void InitializeWindow(const WindowConfig& windowConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler);

	void InitializeGLEW();

	void Initialize(const EngineConfig& engineConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler);

	void Execute();

	GLFWwindow* getWindowHandle() const;

	~GraphicsContext();

private:
	void OnWindowResize(const Size& windowSize);

	std::unique_ptr<Window> m_window{ nullptr };
};
}
