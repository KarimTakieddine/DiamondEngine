#pragma once

#include "GL/glew.h"

#include "config/ComponentConfig.h"
#include "mesh/MeshType.h"

namespace diamond_engine {
class MeshRendererConfig : public ComponentConfig {
public:
	virtual ~MeshRendererConfig() override = default;

	void SetMeshType(MeshType meshType);

	MeshType GetMeshType() const;

	void SetDrawMode(GLenum drawMode);

	GLenum GetDrawMode() const;

	const char* GetName() const override {
		return "MeshRenderer";
	}

private:
	MeshType m_meshType{ MeshType::TRIANGLE };
	GLenum m_drawMode{ GL_STATIC_DRAW };
};
}
