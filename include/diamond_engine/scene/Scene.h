#pragma once

#include "gameobject/GameObjectType.h"
#include "rendering/RenderSequence.h"
#include "shader/SharedShaderStore.h"

namespace diamond_engine {
class Scene {
public:
	explicit Scene(const std::shared_ptr<SharedShaderStore>& sharedShaderStore);

	void SetMaxObjects(GLint maxObjects);

	const std::shared_ptr<GLAllocator>& GetBufferAllocator() const;

	void AddGameObject(std::unique_ptr<GameObject> gameObject, const glm::vec3& position, const glm::vec3& color, GameObjectType type);

	void Update(GLfloat deltaTime);
	void OnWindowResize(int width, int height);

private:
	std::shared_ptr<SharedShaderStore> m_sharedShaderStore	{ nullptr };
	std::shared_ptr<GLAllocator> m_vertexArrayAllocator		{ nullptr };
	std::shared_ptr<GLAllocator> m_bufferAllocator			{ nullptr };
	std::shared_ptr<Camera> m_camera						{ nullptr };
	SharedObjectAllocator m_renderableObjectAllocator		{ nullptr };
	std::unique_ptr<RenderSequence> m_spriteRenderSequence	{ nullptr };
};
}
