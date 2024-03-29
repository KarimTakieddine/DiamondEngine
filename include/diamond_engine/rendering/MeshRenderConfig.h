#pragma once

#include <GL/glew.h>

#include "MeshType.h"

namespace diamond_engine
{
	class MeshRenderConfig
	{
	public:
		void SetMeshType(MeshType meshType);

		MeshType GetMeshType() const;

		void SetDrawMode(GLenum drawMode);

		GLenum GetDrawMode() const;

	private:
		MeshType m_meshType{ MeshType::TRIANGLE };
		GLenum m_drawMode{ GL_STATIC_DRAW };
	};
}
