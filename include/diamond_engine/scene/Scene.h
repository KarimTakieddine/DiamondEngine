#pragma once

#include "GameObjectType.h"
#include "RenderSequence.h"
#include "SharedShaderStore.h"
#include "TextureLoader.h"

namespace diamond_engine {
class Scene {
public:
	explicit Scene(const std::shared_ptr<SharedShaderStore>& sharedShaderStore, const std::shared_ptr<TextureLoader>& sharedTextureLoader);

	void SetMaxObjects(GLint maxObjects);

	const std::shared_ptr<GLAllocator>& GetBufferAllocator() const;

	void AddGameObject(std::unique_ptr<GameObject> gameObject, const MaterialConfig& materialConfig, const glm::vec3& position, GameObjectType type);

	void Update(GLfloat deltaTime);
	void OnWindowResize(int width, int height);

private:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore		{ nullptr };
	std::shared_ptr<TextureLoader> m_sharedTextureLoader		{ nullptr };
	std::shared_ptr<GLAllocator> m_vertexArrayAllocator			{ nullptr };
	std::shared_ptr<GLAllocator> m_bufferAllocator				{ nullptr };
	std::shared_ptr<Camera> m_camera							{ nullptr };
	SharedObjectAllocator m_renderableObjectAllocator			{ nullptr };
	std::unique_ptr<RenderSequence> m_spriteRenderSequence		{ nullptr };
	std::unique_ptr<RenderSequence> m_collider2DRenderSequence	{ nullptr };
};
}
