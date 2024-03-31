#pragma once

#include <memory>
#include <unordered_map>

#include "Mesh.h"
#include "MeshType.h"

namespace diamond_engine
{
class SharedMeshStore
{
public:
	using MeshTable = std::unordered_map<MeshType, std::shared_ptr<Mesh>>;

	static SharedMeshStore& GetInstance();

	const std::shared_ptr<Mesh>& FindMesh(MeshType type);

private:
	SharedMeshStore();

	MeshTable m_store;
};
}
