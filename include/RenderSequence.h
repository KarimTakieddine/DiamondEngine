#pragma once

#include "AlignedAllocator.hpp"
#include "GLAllocator.h"
#include "Camera.h"
#include "GameObject.h"
#include "ShaderProgram.h"

namespace diamond_engine {
using ObjectAllocator		= AlignedAllocator<RenderableObject, 4>;
using SharedObjectAllocator = std::shared_ptr<ObjectAllocator>;

class RenderSequence {
public:
	struct RenderDescriptor {
		std::string projectionUniform;
		std::string viewUniform;
		std::string modelUniform;
		std::string colorUniform;
	};

	using SharedShaderProgram = std::shared_ptr<ShaderProgram>;

	void SetShaderProgram(const SharedShaderProgram& shaderProgram);

	void SetObjectAllocator(const SharedObjectAllocator& objectAllocator);

	void SetVertexArrayAllocator(const std::shared_ptr<GLAllocator>& vertexArrayAllocator);

	void SetRenderDescriptor(const RenderDescriptor& renderDescriptor);

	const RenderDescriptor& GetRenderDescriptor() const;

	void AddGameObject(std::unique_ptr<GameObject> gameObject, const glm::vec3& position, const glm::vec3& color);

	void SetCamera(const std::shared_ptr<Camera> camera);

	void Update(GLfloat deltaTime);

private:
	RenderDescriptor m_renderDescriptor;
	std::vector<std::unique_ptr<GameObject>> m_gameObjects{ };
	std::shared_ptr<Camera> m_camera{ nullptr };
	std::shared_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
	SharedShaderProgram m_shaderProgram{ nullptr };
	SharedObjectAllocator m_objectAllocator{ nullptr };
	GLint m_projectionUniformLocation{ -1 };
	GLint m_viewUniformLocation{ -1 };
};
}
