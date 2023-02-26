#pragma once

#include <memory>
#include <unordered_map>

#include "Mesh.h"

namespace diamond_engine {
enum class Shape : unsigned {
	TRIANGLE	= 0,
	QUAD		= 1,
	CUBE		= 2
};

class SharedMeshStore {
public:
	using MeshTable = std::unordered_map<Shape, std::shared_ptr<Mesh>>;

	SharedMeshStore& GetInstance();

	const std::shared_ptr<Mesh>& FindMesh(Shape shape);

private:
	SharedMeshStore();

	MeshTable m_store;
};
}