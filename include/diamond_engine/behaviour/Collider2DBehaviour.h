#pragma once

#include "Behaviour.h"

namespace diamond_engine {
	class Collider2DBehaviour : public Behaviour {
	public:
		void Update(GLfloat deltaTime) override;

		void setTrackingGameObject(GameObject const * trackingGameObject);

	private:
		GameObject const * m_trackingGameObject{ nullptr };
	};
}
