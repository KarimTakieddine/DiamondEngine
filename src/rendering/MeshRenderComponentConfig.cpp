#include "MeshRenderComponentConfig.h"

namespace diamond_engine
{
	const char* MeshRenderComponentConfig::getName() const
	{
		return "MeshRender";
	}

	MeshType MeshRenderComponentConfig::getMeshType() const
	{
		return m_meshType;
	}

	void MeshRenderComponentConfig::setMeshType(MeshType meshType)
	{
		m_meshType = meshType;
	}

	GLenum MeshRenderComponentConfig::getDrawMode() const
	{
		return m_drawMode;
	}

	void MeshRenderComponentConfig::setDrawMode(GLenum drawMode)
	{
		m_drawMode = drawMode;
	}
}