#include "MeshRenderConfig.h"

namespace diamond_engine
{
	void MeshRenderConfig::SetMeshType(MeshType meshType)
	{
		m_meshType = meshType;
	}

	MeshType MeshRenderConfig::GetMeshType() const
	{
		return m_meshType;
	}

	void MeshRenderConfig::SetDrawMode(GLenum drawMode)
	{
		m_drawMode = drawMode;
	}

	GLenum MeshRenderConfig::GetDrawMode() const
	{
		return m_drawMode;
	}
}
