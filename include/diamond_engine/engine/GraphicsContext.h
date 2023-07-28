#pragma once

#include <memory>

#include <GL/glew.h>

#include "scene/Scene.h"
#include "engine/Window.h"

namespace diamond_engine {
class GraphicsContext {
public:
	void InitializeWindow(const Size& windowSize, const std::string& windowTitle);

	void InitializeGLEW();

	void SetScene(Scene* scene);

	void Execute();

	~GraphicsContext();

private:
	void OnWindowResize(const Size& windowSize);
	
	void OnWindowUpdate(GLfloat deltaTime);

	std::unique_ptr<Window> m_window{ nullptr };
	Scene* m_scene					{ nullptr };
};
}
