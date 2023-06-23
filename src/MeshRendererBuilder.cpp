#include <stdexcept>

#include "MeshRenderer.h"
#include "MeshRendererBuilder.h"
#include "MeshRendererConfig.h"
#include "SharedMeshStore.h"

namespace diamond_engine {
	std::unique_ptr<Component> MeshRendererBuilder::Build(const ComponentConfig* componentConfig) {
		const MeshRendererConfig* meshRendererConfig = dynamic_cast<const MeshRendererConfig*>(componentConfig);

		if (!meshRendererConfig) {
			throw std::runtime_error("Could not convert input ComponentConfig to a MeshRendererConfig instance");
		}

		std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>();

		switch (meshRendererConfig->GetMeshType())
		{
		case MeshType::QUAD: {
			const std::shared_ptr<Mesh> quadMesh = SharedMeshStore::GetInstance().FindMesh(Shape::QUAD);
			meshRenderer->SetDrawMode(meshRendererConfig->GetDrawMode());
			meshRenderer->SetVertexBufferObject(m_bufferAllocator->Get());
			meshRenderer->SetElementBufferObject(m_bufferAllocator->Get());
			meshRenderer->SetMesh(quadMesh);
			break;
		}
		default:
			break;
		}

		return meshRenderer;
	}
}