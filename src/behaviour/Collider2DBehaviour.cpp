#include "ColliderBehaviour.h"
#include "GameObject.h"
#include "Collider2DBehaviour.h"

namespace diamond_engine
{
	void diamond_engine::Collider2DBehaviour::Update(GLfloat deltaTime)
	{
		m_gameObject->GetRenderableObject()->transform = m_trackingGameObject->GetRenderableObject()->transform;
	}

	void Collider2DBehaviour::setTrackingGameObject(GameObject const* trackingGameObject)
	{
		m_trackingGameObject = trackingGameObject;
	}
}