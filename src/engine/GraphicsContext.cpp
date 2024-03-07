#include <stdexcept>

#include "GraphicsContext.h"
#include "Input.h"

namespace diamond_engine {
	void GraphicsContext::InitializeWindow(const WindowConfig& windowConfig) {
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
			std::bind(&GraphicsContext::OnWindowResize, this, std::placeholders::_1),
			std::bind(&GraphicsContext::OnWindowUpdate, this, std::placeholders::_1)));
	}

	void GraphicsContext::InitializeInput(const KeyboardConfig& keyboardConfig, const ControllerConfig& controllerConfig) {
		auto& stateMonitor = input::StateMonitor::GetInstance();

		for (const auto& keyConfig : keyboardConfig.GetKeyConfigs()) {
			stateMonitor.RegisterKeyboardKey(keyConfig.name, keyConfig.code);
		}

		for (const auto& buttonConfig : controllerConfig.getButtonConfigs())
		{
			stateMonitor.RegisterControllerButton(buttonConfig.name, buttonConfig.button);
		}

		for (const auto& joystickConfig : controllerConfig.getJoystickConfigs())
		{
			stateMonitor.RegisterControllerJoystick(joystickConfig.name, joystickConfig.joystick);
		}

		stateMonitor.setJoystickDeadzone(controllerConfig.getJoystickDeadzone());
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

	void GraphicsContext::Initialize(const EngineConfig& engineConfig) {
		InitializeWindow(engineConfig.GetWindowConfig());
		InitializeInput(engineConfig.GetKeyboardConfig(), engineConfig.getControllerConfig());
		InitializeGLEW();
	}

	void GraphicsContext::SetScene(Scene* scene) {
		m_scene = scene;
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

	GraphicsContext::~GraphicsContext() {
		m_window.reset(nullptr);

		glfwTerminate();
	}

	void GraphicsContext::OnWindowResize(const Size& windowSize) {
		if (m_scene) {
			m_scene->OnWindowResize(windowSize.width, windowSize.height);
		}

		glViewport(0, 0, windowSize.width, windowSize.height);
	}

	void GraphicsContext::OnWindowUpdate(GLfloat deltaTime) {
		if (m_scene) {
			m_scene->Update(deltaTime);
		}
	}
}