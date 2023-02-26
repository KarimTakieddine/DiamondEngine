#pragma once

namespace diamond_engine {
class ShaderProgram;

class Component {
public:
	virtual void BindToContext()									= 0;
	virtual void AttachToProgram(ShaderProgram* shaderProgram)		= 0;
	virtual void DetachFromProgram(ShaderProgram* shaderProgram)	= 0;
	virtual void BindToProgram(const ShaderProgram* shaderProgram)	= 0;
	virtual void Render()											= 0;
	virtual void Update()											= 0;
	virtual const char * GetName() const							= 0;

	virtual ~Component() = default;
};
}
