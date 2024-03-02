#include <stdexcept>

#include "Collider2D.h"
#include "GameObject.h"
#include "MeshRendererBuilder.h"

namespace diamond_engine
{
	GLuint Collider2D::getWidth() const
	{
		return m_width;
	}

	void Collider2D::setWidth(GLuint width)
	{
		m_width = width;
	}

	GLuint Collider2D::getHeight() const
	{
		return m_height;
	}

	void Collider2D::setHeight(GLuint height)
	{
		m_height = height;
	}

	ColliderType Collider2D::getColliderType() const
	{
		return m_colliderType;
	}

	void Collider2D::setColliderType(ColliderType colliderType)
	{
		m_colliderType = colliderType;
	}

	void Collider2D::BindToShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram) { }

	void Collider2D::OnAddedToScene() { }

	const char* Collider2D::GetName() const
	{
		return "Collider2D";
	}
}