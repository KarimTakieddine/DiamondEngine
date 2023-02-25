#pragma once

namespace diamond_engine {
class ShaderProgram;

class Component {
public:
	virtual void BindToContext()										= 0;
	virtual void AttachToProgram(const ShaderProgram* shaderProgram)	= 0;
	virtual void BindToProgram(const ShaderProgram* shaderProgram)		= 0;
	virtual void Render()												= 0;
	virtual void Update()												= 0;

	virtual ~Component() = default;
};
}
