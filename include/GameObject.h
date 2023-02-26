#pragma once

#include <memory>
#include <vector>

#include "Component.h"
#include "ShaderProgram.h"

namespace diamond_engine {
class Component;
class ShaderProgram;

class GameObject {
public:
	GameObject();

	void AcquireComponent(std::unique_ptr<Component> component);
	void ReleaseComponent(std::unique_ptr<Component> component);

private:
	std::vector<std::unique_ptr<Component>> m_components;
	std::unique_ptr<ShaderProgram> m_shaderProgram{ nullptr };
};
}
