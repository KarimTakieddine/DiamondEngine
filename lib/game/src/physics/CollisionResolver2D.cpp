#include <algorithm>
#include <cmath>

#include <glm/gtx/vec_swizzle.hpp>

#include "Collider2DComponent.h"
#include "CollisionResolver2D.h"
#include "GameInstance.h"

namespace diamond_engine
{
	void CollisionResolver2D::addCharacterSprite(GameInstance* characterSprite)
	{
		m_characterSprites.push_back(characterSprite);
	}

	void CollisionResolver2D::addObstacleSprite(GameInstance* obstacleSprite)
	{
		m_obstacleSprites.push_back(obstacleSprite);
	}

	void CollisionResolver2D::ResolveCollisions()
	{
		for (auto const characterSprite : m_characterSprites)
		{
			Collider2DComponent* characterCollider = characterSprite->getBehaviourComponent<Collider2DComponent>("Collider2D");

			const glm::mat4& characterLocalScale	= characterCollider->getRenderObject()->transform.getLocalScale();
			const glm::vec2 characterColliderSize	= { characterLocalScale[0].x, characterLocalScale[1].y };
			const GLfloat characterHalfWidth		= characterColliderSize.x * 0.5f;
			const GLfloat characterHalfHeight		= characterColliderSize.y * 0.5f;

			const Transform& characterTransform = characterCollider->getTarget()->transform;
			const glm::vec2 characterPosition	= glm::xy(characterTransform.getPosition());

			AxisAlignedBoundingBox characterAABB{
				characterPosition + glm::xy(characterTransform.getLocalRotation() * glm::vec4(-characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform.getLocalRotation() * glm::vec4(characterHalfWidth, characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform.getLocalRotation() * glm::vec4(characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f)),
				characterPosition + glm::xy(characterTransform.getLocalRotation() * glm::vec4(-characterHalfWidth, -characterHalfHeight, 0.0f, 1.0f))
			};

			for (auto const obstacleSprite : m_obstacleSprites)
			{
				Collider2DComponent* obstacleCollider = obstacleSprite->getBehaviourComponent<Collider2DComponent>("Collider2D");

				const glm::mat4& obstacleLocalScale		= obstacleCollider->getRenderObject()->transform.getLocalScale();
				const glm::vec2 obstacleColliderSize	= { obstacleLocalScale[0].x, obstacleLocalScale[1].y };
				const GLfloat obstacleHalfWidth			= obstacleColliderSize.x * 0.5f;
				const GLfloat obstacleHalfHeight		= obstacleColliderSize.y * 0.5f;

				const Transform& obstacleTransform	= obstacleCollider->getTarget()->transform;
				const glm::vec2 obstaclePosition	= glm::xy(obstacleTransform.getPosition());

				AxisAlignedBoundingBox obstacleAABB{
					obstaclePosition + glm::xy(obstacleTransform.getLocalRotation() * glm::vec4(-obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform.getLocalRotation() * glm::vec4(obstacleHalfWidth, obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform.getLocalRotation() * glm::vec4(obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f)),
					obstaclePosition + glm::xy(obstacleTransform.getLocalRotation() * glm::vec4(-obstacleHalfWidth, -obstacleHalfHeight, 0.0f, 1.0f))
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
						/*if (m_collisionResolutionMap.find(obstacleSprite->getName()) != m_collisionResolutionMap.cend())
						{
							m_collisionResolutionMap.erase(obstacleSprite->getName());
							characterSprite->OnCollisionExit(obstacleSprite->getName());
						}*/

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

					characterSprite->getRenderObject()->transform.Translate(resolutionVector);

					/*if (m_collisionResolutionMap.find(obstacleSprite->getName()) == m_collisionResolutionMap.cend())
					{
						m_collisionResolutionMap.insert({ obstacleSprite->getName(), characterSprite->getName() });
						characterSprite->OnCollisionEnter(glm::normalize(resolutionVector), obstacleSprite->getName());
					}*/
				}
			}
		}
	}
}