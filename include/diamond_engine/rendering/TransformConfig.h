#pragma once

#include <glm/vec3.hpp>

namespace diamond_engine
{
	class TransformConfig
	{
	public:
		const glm::vec3& getPosition() const;
		glm::vec3& getPosition();
		void setPosition(const glm::vec3& position);

	private:
		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	};
}
