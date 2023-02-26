#pragma once 

#include <memory>
#include <string>

#include <GL/glew.h>

#include "Component.h"

namespace diamond_engine {
enum class MeshType : unsigned {
	BACKGROUND	= 0,
	SPRITE		= 1
};

enum class RenderMode : unsigned {
	SOLID		= 0,
	WIREFRAME	= 1
};

inline GLenum GetDrawType(MeshType meshType) {
	switch (meshType) {
	case MeshType::BACKGROUND:
		return GL_STATIC_DRAW;
	case MeshType::SPRITE:
		return GL_DYNAMIC_DRAW;
	default:
		return GL_STATIC_DRAW;
	}
}

inline GLenum GetDrawMode(RenderMode renderMode) {
	switch (renderMode) {
	case RenderMode::SOLID:
		return GL_TRIANGLES;
	case RenderMode::WIREFRAME:
		return GL_LINES;
	default:
		return GL_TRIANGLES;
	}
}

class Mesh;

class MeshRenderer : public Component {
public:
	static const std::string kVertexAttributeLocation;
	static const std::string kColorAttributeLocation;

	MeshRenderer(
		GLuint vertexArrayObject,
		GLuint vertexBufferObject,
		GLuint elementBufferObject);

	void BindToContext() override;

	void Render() override;

	void Update() override { }

	void SetMeshType(MeshType meshType);

	void SetRenderMode(RenderMode renderMode);

	void SetMesh(const std::shared_ptr<Mesh>& mesh);

private:
	std::shared_ptr<Mesh> m_mesh{ nullptr };
	MeshType m_meshType			{ MeshType::BACKGROUND };
	RenderMode m_renderMode		{ RenderMode::SOLID };
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferObject;
};
}
