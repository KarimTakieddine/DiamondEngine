#include <algorithm>
#include <cmath>

#include <glm/gtx/vec_swizzle.hpp>

#include "Collider2DComponent.h"
#include "CollisionSolver2D.h"
#include "GameInstance.h"
#include "TransformRenderComponent.h"

namespace diamond_engine
{
	void CollisionSolver2D::addCharacter(GameInstance* character)
	{
		m_characters.push_back(character);
	}

	void CollisionSolver2D::addObstacle(GameInstance* obstacle)
	{
		m_obstacles.push_back(obstacle);
	}

	void CollisionSolver2D::ResolveCollisions()
	{
		for (auto character : m_characters)
		{
			Collider2DComponent* characterCollider = character->getBehaviourComponent<Collider2DComponent>("Collider2D");

			TransformRenderComponent* characterTransform	= characterCollider->getTarget()->getRenderComponent<TransformRenderComponent>("Transform");
			const glm::mat4& characterLocalScale			= characterTransform->getLocalScale();
			const Size& characterColliderSize				= characterCollider->getSize();
			const GLfloat characterHalfWidth				= characterLocalScale[0].x * characterColliderSize.width * 0.5f;
			const GLfloat characterHalfHeight				= characterLocalScale[1].y * characterColliderSize.height * 0.5f;
			const glm::vec2 characterPosition				= glm::xy(characterTransform->getPosition());

			AxisAlignedBoundingBox characterAABB{
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform->getLocalRotation() * glm::vec4(-characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f))
			};

			for (auto obstacle : m_obstacles)
			{
				Collider2DComponent* obstacleCollider = obstacle->getBehaviourComponent<Collider2DComponent>("Collider2D");

				TransformRenderComponent* obstacleTransform = obstacleCollider->getTarget()->getRenderComponent<TransformRenderComponent>("Transform");
				const glm::mat4& obstacleLocalScale			= obstacleTransform->getLocalScale();
				const Size& obstacleColliderSize			= obstacleCollider->getSize();
				const GLfloat obstacleHalfWidth				= obstacleLocalScale[0].x * obstacleColliderSize.width * 0.5f;
				const GLfloat obstacleHalfHeight			= obstacleLocalScale[1].y * obstacleColliderSize.height * 0.5f;
				const glm::vec2 obstaclePosition			= glm::xy(obstacleTransform->getPosition());

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
						if (m_collisionResolutionMap.find(obstacle->getInternalName()) != m_collisionResolutionMap.cend())
						{
							m_collisionResolutionMap.erase(obstacle->getInternalName());
							character->onCollisionExit2D(obstacle->getInternalName());
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

					if (m_collisionResolutionMap.find(obstacle->getInternalName()) == m_collisionResolutionMap.cend())
					{
						m_collisionResolutionMap.insert({ obstacle->getInternalName(), character->getInternalName() });
						character->onCollisionEnter2D(glm::normalize(resolutionVector), obstacle->getInternalName());
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