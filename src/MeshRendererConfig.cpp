#include "MeshRendererConfig.h"

namespace diamond_engine {
	void MeshRendererConfig::SetMeshType(MeshType meshType) {
		m_meshType = meshType;
	}

	MeshType MeshRendererConfig::GetMeshType() const {
		return m_meshType;
	}
}