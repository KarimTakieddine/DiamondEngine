#pragma once

#include <glm/vec3.hpp>

#include "RenderComponentConfig.h"

namespace diamond_engine
{
	class TransformComponentConfig : public RenderComponentConfig
	{
	public:
		~TransformComponentConfig() final override = default;
		const char* getName() const final override;

		const glm::vec3& getPosition() const;
		void setPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	};
}
