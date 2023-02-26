#pragma once

#include <memory>
#include <unordered_map>

#include "Mesh.h"

namespace diamond_engine {
class SharedMeshStore {
public:
	enum class Geometry : unsigned {
		TRIANGLE	= 0,
		QUAD		= 1,
		CUBE		= 2
	};

	using MeshTable = std::unordered_map<Geometry, std::shared_ptr<Mesh>>;

	SharedMeshStore& GetInstance();

	const std::shared_ptr<Mesh>& FindMesh(Geometry geometry);

private:
	SharedMeshStore();

	MeshTable m_store;
};
}
