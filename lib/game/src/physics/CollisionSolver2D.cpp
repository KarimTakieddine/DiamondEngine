#include <algorithm>
#include <cmath>
#include <functional>
#include <map>

#include <glm/gtx/vec_swizzle.hpp>

#include "Collider2DComponent.h"
#include "CollisionSolver2D.h"
#include "GameInstance.h"
#include "TransformRenderComponent.h"

namespace
{
	using ColliderIgnoreFlags = diamond_engine::ColliderIgnoreFlags;

	const std::map<ColliderIgnoreFlags, std::function<bool(const glm::vec2&)>> kColliderIgnoreChecks =
	{
		{ ColliderIgnoreFlags::LEFT,	[](const glm::vec2& input) { return input.x < 0.0f; } },
		{ ColliderIgnoreFlags::RIGHT,	[](const glm::vec2& input) { return input.x > 0.0f; } },
		{ ColliderIgnoreFlags::UP,		[](const glm::vec2& input) { return input.y > 0.0f; } },
		{ ColliderIgnoreFlags::DOWN,	[](const glm::vec2& input) { return input.y < 0.0f; } }
	};

	void filterResolutionVectors(ColliderIgnoreFlags ignoreFlags, std::vector<glm::vec2>& output)
	{
		for (const auto& pair : kColliderIgnoreChecks)
		{
			const ColliderIgnoreFlags ignoreFlag = pair.first;

			if ( (ignoreFlags & ignoreFlag) == ignoreFlag )
			{
				output.erase(std::remove_if(output.begin(), output.end(), pair.second));
			}
		}
	}
}

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
			Collider2DComponent* characterCollider			= character.collider;
			const glm::vec2& characterColliderSize			= characterCollider->getSize();
			const GLfloat characterHalfWidth				= characterColliderSize.x * 0.5f;
			const GLfloat characterHalfHeight				= characterColliderSize.y * 0.5f;
			const glm::vec2 characterPosition				= glm::xy(characterTransform->getPosition()) + characterCollider->getOffset();

			AxisAlignedBoundingBox characterAABB{
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f))
			};

			auto* characterInstance = character.gameInstance;
			const std::string& characterName = characterInstance->getInternalName();

			for (const auto& obstacle : m_obstacles)
			{
				TransformRenderComponent* obstacleTransform = obstacle.transform;
				Collider2DComponent* obstacleCollider		= obstacle.collider;
				const glm::vec2& obstacleColliderSize		= obstacleCollider->getSize();
				const GLfloat obstacleHalfWidth				= obstacleColliderSize.x * 0.5f;
				const GLfloat obstacleHalfHeight			= obstacleColliderSize.y * 0.5f;
				const glm::vec2 obstaclePosition			= glm::xy(obstacleTransform->getPosition()) + obstacleCollider->getOffset();

				AxisAlignedBoundingBox obstacleAABB{
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(-obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform->getLocalRotation() * glm::vec4(-obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f))
				};

				auto* obstacleInstance = obstacle.gameInstance;
				const std::string& obstacleName = obstacleInstance->getInternalName();

				const std::vector<glm::vec2> axes{
					glm::vec2{ 1.0f, 0.0f },
					glm::vec2{ 0.0f, 1.0f }
				};

				bool colliding = true;

				std::vector<glm::vec2> penetrations;

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
						if (m_collisionResolutionMap.find(obstacleName) != m_collisionResolutionMap.cend())
						{
							m_collisionResolutionMap.erase(obstacleName);

							character.gameInstance->onCollisionExit2D(obstacleInstance, obstacleCollider);
							obstacle.gameInstance->onCollisionExit2D(characterInstance, characterCollider);
						}

						colliding = false;
						break;
					}

					penetrations.push_back(axis * ( -(characterMaximum - obstacleMinimum) ) );
					penetrations.push_back(axis * ( obstacleMaximum - characterMinimum ) );
				}

				if (colliding)
				{
					::filterResolutionVectors(obstacleCollider->getIgnoreFlags(), penetrations);

					std::vector<GLfloat> absolutePenetrations;

					for (const auto& penetration : penetrations)
					{
						absolutePenetrations.push_back(glm::length(penetration));
					}

					const GLfloat minimumPenetration = *std::min_element(absolutePenetrations.begin(), absolutePenetrations.end());
					glm::vec2 resolutionVector = { };

					for (const auto& penetration : penetrations)
					{
						if (glm::length(penetration) == minimumPenetration)
						{
							resolutionVector = penetration;
							break;
						}
					}

					if (obstacleCollider->getType() != ColliderType::ZONE)
					{
						characterTransform->translate(resolutionVector);
					}

					if (m_collisionResolutionMap.find(obstacleName) == m_collisionResolutionMap.cend())
					{
						m_collisionResolutionMap.insert({ obstacleName, characterName });

						const glm::vec2 normalisedResolution = glm::normalize(resolutionVector);
						character.gameInstance->onCollisionEnter2D(normalisedResolution, obstacleInstance, obstacleCollider);
						obstacle.gameInstance->onCollisionEnter2D(normalisedResolution, characterInstance, characterCollider);
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