#pragma once

#include <memory>

#include <GL/glew.h>

#include "EngineConfig.h"
#include "Window.h"

namespace diamond_engine {
class GraphicsContext {
public:
	void InitializeWindow(const WindowConfig& windowConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler);

	void InitializeGLEW();

	void Initialize(const EngineConfig& engineConfig, Window::UpdateHandler updateHandler, Window::ResizeHandler resizeHandler);

	void Execute();

	const std::unique_ptr<Window>& getWindow() const;

	~GraphicsContext();

private:
	std::unique_ptr<Window> m_window{ nullptr };
};
}
