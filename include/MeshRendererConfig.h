#pragma once

#include "GL/glew.h"

#include "ComponentConfig.h"
#include "MeshType.h"

namespace diamond_engine {
class MeshRendererConfig : public ComponentConfig {
public:
	virtual ~MeshRendererConfig() override = default;

	ComponentType GetType() const override {
		return ComponentType::MeshRenderer;
	}

	void SetMeshType(MeshType meshType);

	MeshType GetMeshType() const;

	void SetDrawMode(GLenum drawMode);

	GLenum GetDrawMode() const;

private:
	MeshType m_meshType;
	GLenum m_drawMode;
};
}
