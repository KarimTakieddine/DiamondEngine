#pragma once

#include <memory>
#include <unordered_map>

#include "GLAllocator.h"
#include "Mesh.h"
#include "MeshType.h"

namespace diamond_engine
{
	class SharedMeshStore
	{
	public:
		using MeshDataTable = std::unordered_map<MeshType, MeshData>;
		using MeshStore		= std::unordered_map<MeshType, std::unique_ptr<Mesh>>;

		static const MeshDataTable kBaseMeshData;

		static constexpr size_t MESH_COUNT = 4;

		const MeshType availableMeshes[MESH_COUNT] =
		{
			MeshType::TRIANGLE,
			MeshType::QUAD,
			MeshType::CUBE,
			MeshType::COLLIDER
		};

		static std::shared_ptr<SharedMeshStore>& getInstance();

		void setMaxMeshCount(GLsizei count);

		void loadMeshes();

		void unloadMeshes();

		void registerMesh(MeshType type, const MeshData& data);

		Mesh* FindMesh(MeshType type);

	private:
		SharedMeshStore();

		MeshStore m_store;

		std::unique_ptr<GLAllocator> m_bufferAllocator{ nullptr };
	};
}
