#pragma once

#include <GL/glew.h>

#include "RenderComponentConfig.h"
#include "MeshType.h"

namespace diamond_engine
{
	class MeshRenderComponentConfig : public RenderComponentConfig
	{
	public:
		~MeshRenderComponentConfig() final override = default;
		const char* getName() const final override;

		MeshType getMeshType() const;
		void setMeshType(MeshType meshType);

		GLenum getDrawMode() const;
		void setDrawMode(GLenum drawMode);

	private:
		MeshType m_meshType{ MeshType::TRIANGLE };
		GLenum m_drawMode{ GL_STATIC_DRAW };
	};
}
