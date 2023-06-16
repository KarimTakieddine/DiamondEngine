#pragma once

#include "AlignedAllocator.hpp"
#include "Camera.h"
#include "GameObject.h"
#include "ShaderProgram.h"

namespace diamond_engine {
	class RenderSequence {
	public:
		struct RenderDescriptor {
			std::string projectionUniform;
			std::string viewUniform;
			std::string modelUniform;
			std::string colorUniform;
		};

		using SharedShaderProgram	= std::shared_ptr<ShaderProgram>;
		using SharedObjectAllocator = std::shared_ptr<AlignedAllocator<RenderableObject, 4>>;

		void SetShaderProgram(const SharedShaderProgram& shaderProgram);

		void SetObjectAllocator(const SharedObjectAllocator& objectAllocator);

		void SetRenderDescriptor(const RenderDescriptor& renderDescriptor);

		const RenderDescriptor& GetRenderDescriptor() const;

		void AddGameObject(std::unique_ptr<GameObject> gameObject);

		void SetCamera(const std::shared_ptr<Camera> camera);

		void Update(GLfloat deltaTime);

	private:
		RenderDescriptor m_renderDescriptor;
		std::vector<std::unique_ptr<GameObject>> m_gameObjects{ };
		std::shared_ptr<Camera> m_camera{ nullptr };
		SharedShaderProgram m_shaderProgram{ nullptr };
		SharedObjectAllocator m_objectAllocator{ nullptr };
		GLint m_projectionUniformLocation{ -1 };
		GLint m_viewUniformLocation{ -1 };
	};
}
