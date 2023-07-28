#include <stdexcept>

#include "component/MeshRenderer.h"
#include "builder/MeshRendererBuilder.h"
#include "config/MeshRendererConfig.h"
#include "mesh/SharedMeshStore.h"

namespace diamond_engine {
	/* static */ std::unique_ptr<Component> MeshRendererBuilder::Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const ComponentConfig* componentConfig) {
		const MeshRendererConfig* meshRendererConfig = dynamic_cast<const MeshRendererConfig*>(componentConfig);

		if (!meshRendererConfig) {
			throw std::runtime_error("Could not convert input ComponentConfig to a MeshRendererConfig instance");
		}

		std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>();

		meshRenderer->SetDrawMode(meshRendererConfig->GetDrawMode());
		meshRenderer->SetVertexBufferObject(bufferAllocator->Get());
		meshRenderer->SetElementBufferObject(bufferAllocator->Get());

		switch (meshRendererConfig->GetMeshType())
		{
		case MeshType::QUAD: {
			const std::shared_ptr<Mesh> quadMesh = SharedMeshStore::GetInstance().FindMesh(Shape::QUAD);
			meshRenderer->SetMesh(quadMesh);
			break;
		}
		case MeshType::CUBE: {
			const std::shared_ptr<Mesh> cubeMesh = SharedMeshStore::GetInstance().FindMesh(Shape::CUBE);
			meshRenderer->SetMesh(cubeMesh);
			break;
		}
		default:
			break;
		}

		return meshRenderer;
	}
}