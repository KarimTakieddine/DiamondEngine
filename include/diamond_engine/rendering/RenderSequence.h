#pragma once

#include <optional>

#include "AlignedAllocator.hpp"
#include "GLAllocator.h"
#include "Camera.h"
#include "GameObject.h"
#include "MaterialConfig.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"

namespace diamond_engine {
using ObjectAllocator		= AlignedAllocator<RenderableObject, 4>;
using SharedObjectAllocator = std::shared_ptr<ObjectAllocator>;

class RenderSequence {
public:
	struct RenderDescriptor {
		std::string projectionUniform;
		std::string cameraTranslationUniform;
		std::string cameraRotationUniform;
		std::string cameraScaleUniform;
		std::string cameraViewUniform;
		std::string objectModelUniform;
		std::string colorUniform;
		std::string textureOffsetUniform;
	};

	using SharedShaderProgram = std::shared_ptr<ShaderProgram>;

	void SetShaderProgram(const SharedShaderProgram& shaderProgram);

	void SetObjectAllocator(const SharedObjectAllocator& objectAllocator);

	void SetVertexArrayAllocator(const std::shared_ptr<GLAllocator>& vertexArrayAllocator);

	void SetTextureLoader(const std::shared_ptr<TextureLoader>& textureLoader);

	void SetRenderDescriptor(const RenderDescriptor& renderDescriptor);

	const RenderDescriptor& GetRenderDescriptor() const;

	void AddGameObject(std::unique_ptr<GameObject> gameObject, const MaterialConfig& materialConfig, const glm::vec3& position, const glm::vec3& scale);

	void SetCamera(const std::shared_ptr<Camera> camera);

	void Update(GLfloat deltaTime);

	void Render();

private:
	RenderDescriptor m_renderDescriptor;
	std::vector<std::unique_ptr<GameObject>> m_gameObjects{ };
	std::shared_ptr<Camera> m_camera{ nullptr };
	std::shared_ptr<GLAllocator> m_vertexArrayAllocator{ nullptr };
	std::shared_ptr<TextureLoader> m_textureLoader{ nullptr };
	SharedShaderProgram m_shaderProgram{ nullptr };
	SharedObjectAllocator m_objectAllocator{ nullptr };
	GLint m_projectionUniformLocation{ -1 };
	GLint m_cameraTranslationUniformLocation{ -1 };
	GLint m_cameraRotationUniformLocation{ -1 };
	GLint m_cameraScaleUniformLocation{ -1 };
	GLint m_cameraViewUniformLocation{ -1 };
};
}
