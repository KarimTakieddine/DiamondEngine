#pragma once

#include <vector>
#include <unordered_map>

#include <GL/glew.h>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

namespace diamond_engine
{
	struct AxisAlignedBoundingBox
	{
		glm::vec2 topLeft{ 0.0f, 0.0f };
		glm::vec2 topRight{ 0.0f, 0.0f };
		glm::vec2 bottomRight{ 0.0f, 0.0f };
		glm::vec2 bottomLeft{ 0.0f, 0.0f };
	};

	class GameObject;
	class CollisionResolver2D
	{
	public:
		void addCharacterSprite(GameObject* characterSprite);

		void addObstacleSprite(GameObject* obstacleSprite);

		void ResolveCollisions();

	private:
		std::unordered_map<std::string, std::string> m_collisionResolutionMap;
		std::vector<GameObject*> m_characterSprites;
		std::vector<GameObject*> m_obstacleSprites;
	};
}
