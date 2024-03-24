#pragma once

#include "CollisionResolver2D.h"
#include "GameObjectType.h"
#include "RenderSequence.h"
#include "SharedShaderStore.h"
#include "TextureLoader.h"
#include "SpriteSheetLoader.h"

namespace diamond_engine {
class Scene {
public:
	Scene(const std::shared_ptr<SharedShaderStore>& sharedShaderStore, const std::shared_ptr<TextureLoader>& sharedTextureLoader, const std::shared_ptr<SpriteSheetLoader>& spriteSheetLoader);

	void SetMaxObjects(GLint maxObjects);

	const std::shared_ptr<GLAllocator>& GetBufferAllocator() const;

	void AddGameObject(std::unique_ptr<GameObject> gameObject, const MaterialConfig& materialConfig, const glm::vec3& position, const glm::vec3& scale, GameObjectType type);

	void Update(GLfloat deltaTime);
	void OnWindowResize(int width, int height);

private:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore		{ nullptr };
	std::shared_ptr<TextureLoader> m_sharedTextureLoader		{ nullptr };
	std::shared_ptr<SpriteSheetLoader> m_sharedSpriteSheetLoader{ nullptr };
	std::shared_ptr<GLAllocator> m_vertexArrayAllocator			{ nullptr };
	std::shared_ptr<GLAllocator> m_bufferAllocator				{ nullptr };
	std::shared_ptr<Camera> m_camera							{ nullptr };
	SharedObjectAllocator m_renderableObjectAllocator			{ nullptr };
	std::unique_ptr<RenderSequence> m_spriteRenderSequence		{ nullptr };
	std::unique_ptr<RenderSequence> m_collider2DRenderSequence	{ nullptr };
	std::unique_ptr<CollisionResolver2D> m_collisionResolver2D	{ nullptr };
};
}
