#pragma once

#include <memory>

#include <GL/glew.h>

#include "EngineConfig.h"
#include "Scene.h"
#include "Window.h"

namespace diamond_engine {
class GraphicsContext {
public:
	void InitializeWindow(const WindowConfig& windowConfig);

	void InitializeInput(const KeyboardConfig& keyboardConfig);

	void InitializeGLEW();

	void Initialize(const EngineConfig& engineConfig);

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
