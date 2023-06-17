#pragma once

#include "ComponentConfig.h"
#include "MeshType.h"

namespace diamond_engine {
class MeshRendererConfig : public ComponentConfig {
public:
	virtual ~MeshRendererConfig() override = default;

	ComponentType GetType() const {
		return ComponentType::MeshRenderer;
	}

	void SetMeshType(MeshType meshType);

	MeshType GetMeshType() const;

private:
	MeshType m_meshType;
};
}
