#pragma once 

#include <memory>

#include "Component.h"
#include "Mesh.h"

namespace diamond_engine {
class MeshRenderer : public Component {
public:
	static const std::string kVertexAttributeName;
	static const std::string kColorAttributeName;
	static const std::string kTextureCoordinateName;

	void BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) override;

	void OnAddedToScene() override;

	const char* GetName() const override {
		return "MeshRenderer";
	}

	void SetMesh(const std::shared_ptr<Mesh>& mesh);

	void SetVertexBufferObject(GLuint vertexBufferObject);

	void SetElementBufferObject(GLuint elementBufferObject);

	void SetVertexAttributeLocation(GLuint vertexAttributeLocation);

	void SetColorAttributeLocation(GLuint colorAttributeLocation);

	void SetDrawMode(GLenum drawMode);

private:
	std::shared_ptr<Mesh> m_mesh	{ nullptr };
	GLuint m_vertexArrayObject		{ 0 };
	GLuint m_vertexBufferObject		{ 0 };
	GLuint m_elementBufferObject	{ 0 };
	GLint m_vertexAttributeLocation	{ -1 };
	GLint m_colorAttributeLocation	{ -1 };
	GLenum m_drawMode				{ GL_STATIC_DRAW };
};
}
