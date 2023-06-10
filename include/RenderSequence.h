#pragma once

#include "GameObject.h"
#include "ShaderProgram.h"

namespace diamond_engine {
	class RenderSequence {
	public:
		using SharedShaderProgram = std::shared_ptr<ShaderProgram>;

		void SetShaderProgram(const SharedShaderProgram& shaderProgram);

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects{ };
		SharedShaderProgram m_shaderProgram{ nullptr };
	};
}
