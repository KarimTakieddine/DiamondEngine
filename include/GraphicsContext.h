#pragma once

#include <memory>

#include <GL/glew.h>

#include "Scene.h"
#include "Window.h"

namespace diamond_engine {
class GraphicsContext {
public:
	void InitializeWindow(const Size& windowSize, const std::string& windowTitle);

	void InitializeGLEW();

	void SetScene(std::unique_ptr<Scene> scene);

	void Execute();

	~GraphicsContext();

private:
	void OnWindowResize(const Size& windowSize);
	
	void OnWindowUpdate(GLfloat deltaTime);

	std::unique_ptr<Window> m_window{ nullptr };
	std::unique_ptr<Scene> m_scene{ nullptr };
};
}
