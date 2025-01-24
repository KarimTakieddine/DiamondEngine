#include <algorithm>
#include <cmath>

#include <glm/gtx/vec_swizzle.hpp>

#include "Collider2DComponent.h"
#include "CollisionSolver2D.h"
#include "GameInstance.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	void CollisionSolver2D::addCharacter(GameInstance* character, Collider2DComponent* collider)
	{
		m_characters.push_back({
			character,
			character->getRenderComponent<TransformRenderComponent>("Transform"),
			collider });
	}

	void CollisionSolver2D::addObstacle(GameInstance* obstacle, Collider2DComponent* collider)
	{
		m_obstacles.push_back({
			obstacle,
			obstacle->getRenderComponent<TransformRenderComponent>("Transform"),
			collider });
	}

	void CollisionSolver2D::ResolveCollisions()
	{
		for (const auto& character : m_characters)
		{
			TransformRenderComponent* characterTransform	= character.transform;
			const glm::vec2& characterColliderSize			= character.collider->getSize();
			const GLfloat characterHalfWidth				= characterColliderSize.x * 0.5f;
			const GLfloat characterHalfHeight				= characterColliderSize.y * 0.5f;
			const glm::vec2 characterPosition				= glm::xy(characterTransform->getPosition()) + character.collider->getOffset();

			AxisAlignedBoundingBox characterAABB{
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f))
			};

			for (const auto& obstacle : m_obstacles)
			{
				TransformRenderComponent* obstacleTransform = obstacle.transform;
				const glm::vec2& obstacleColliderSize		= obstacle.collider->getSize();
				const GLfloat obstacleHalfWidth				= obstacleColliderSize.x * 0.5f;
				const GLfloat obstacleHalfHeight			= obstacleColliderSize.y * 0.5f;
				const glm::vec2 obstaclePosition			= glm::xy(obstacleTransform->getPosition()) + obstacle.collider->getOffset();

				AxisAlignedBoundingBox obstacleAABB{
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(-obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(-obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f))
				};

				const std::vector<glm::vec2> axes{
					glm::vec2{ 1.0f, 0.0f },
					glm::vec2{ 0.0f, 1.0f }
				};

				bool colliding = true;

				std::vector<GLfloat> penetrations;

				for (const auto& axis : axes)
				{
					std::vector<GLfloat> characterProjections{
						glm::dot(characterAABB.topLeft, axis),
						glm::dot(characterAABB.topRight, axis),
						glm::dot(characterAABB.bottomRight, axis),
						glm::dot(characterAABB.bottomLeft, axis)
					};

					std::vector<GLfloat> obstacleProjections{
						glm::dot(obstacleAABB.topLeft, axis),
						glm::dot(obstacleAABB.topRight, axis),
						glm::dot(obstacleAABB.bottomRight, axis),
						glm::dot(obstacleAABB.bottomLeft, axis)
					};

					const GLfloat characterMaximum	= *std::max_element(characterProjections.begin(), characterProjections.end());
					const GLfloat characterMinimum	= *std::min_element(characterProjections.begin(), characterProjections.end());
					const GLfloat obstacleMaximum	= *std::max_element(obstacleProjections.begin(), obstacleProjections.end());
					const GLfloat obstacleMinimum	= *std::min_element(obstacleProjections.begin(), obstacleProjections.end());

					if (obstacleMinimum > characterMaximum || characterMinimum > obstacleMaximum)
					{
						if (m_collisionResolutionMap.find(obstacle.gameInstance->getInternalName()) != m_collisionResolutionMap.cend())
						{
							m_collisionResolutionMap.erase(obstacle.gameInstance->getInternalName());
							character.gameInstance->onCollisionExit2D(obstacle.gameInstance->getInternalName());
						}

						colliding = false;
						break;
					}

					penetrations.push_back(-(characterMaximum - obstacleMinimum));
					penetrations.push_back(obstacleMaximum - characterMinimum);
				}

				if (colliding)
				{
					std::vector<GLfloat> absolutePenetrations;

					for (GLfloat penetration : penetrations)
					{
						absolutePenetrations.push_back(std::abs(penetration));
					}

					const GLfloat minimumPenetration	= *std::min_element(absolutePenetrations.begin(), absolutePenetrations.end());
					glm::vec2 resolutionVector			= { };

					if (minimumPenetration == absolutePenetrations[0])
					{
						resolutionVector = axes[0] * penetrations[0];
					}
					else if (minimumPenetration == absolutePenetrations[1])
					{
						resolutionVector = axes[0] * penetrations[1];
					}
					else if (minimumPenetration == absolutePenetrations[2])
					{
						resolutionVector = axes[1] * penetrations[2];
					}
					else if (minimumPenetration == absolutePenetrations[3])
					{
						resolutionVector = axes[1] * penetrations[3];
					}

					characterTransform->translate(resolutionVector);

					if (m_collisionResolutionMap.find(obstacle.gameInstance->getInternalName()) == m_collisionResolutionMap.cend())
					{
						m_collisionResolutionMap.insert({ obstacle.gameInstance->getInternalName(), character.gameInstance->getInternalName() });
						character.gameInstance->onCollisionEnter2D(glm::normalize(resolutionVector), obstacle.gameInstance->getInternalName());
					}
				}
			}
		}
	}

	void CollisionSolver2D::clear()
	{
		m_characters.clear();
		m_obstacles.clear();
	}
}