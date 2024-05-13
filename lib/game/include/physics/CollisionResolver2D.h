#pragma once

#include <vector>
#include <string>
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

	class GameInstance;
	class CollisionResolver2D
	{
	public:
		void addCharacterSprite(GameInstance* characterSprite);

		void addObstacleSprite(GameInstance* obstacleSprite);

		void ResolveCollisions();

		void clear();

	private:
		std::unordered_map<std::string, std::string> m_collisionResolutionMap;
		std::vector<GameInstance*> m_characterSprites;
		std::vector<GameInstance*> m_obstacleSprites;
	};
}
