#include <stdexcept>
#include <string>

#include "SharedMeshStore.h"

namespace diamond_engine {
	/* static */ std::shared_ptr<SharedMeshStore>& SharedMeshStore::getInstance()
	{
		static std::shared_ptr<SharedMeshStore> sharedMeshStore(new SharedMeshStore());
		return sharedMeshStore;
	}

	void SharedMeshStore::loadMeshes()
	{
		m_bufferAllocator->Allocate(MESH_COUNT << 1);

		for (size_t i = 0; i < MESH_COUNT; ++i)
		{
			const MeshType meshType = availableMeshes[i];

			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(m_dataStore.at(meshType));

			mesh->setVertexBufferObject(m_bufferAllocator->Get());
			mesh->setElementBufferObject(m_bufferAllocator->Get());

			m_store.insert({ meshType, std::move(mesh)});
		}
	}

	void SharedMeshStore::unloadMeshes()
	{
		m_store.clear();
		m_bufferAllocator->Free(m_bufferAllocator->GetAllocatedObjectCount());
	}

	Mesh* SharedMeshStore::FindMesh(MeshType type) {
		auto meshIt = m_store.find(type);

		if (meshIt == m_store.cend())
		{
			return nullptr;
		}

		return meshIt->second.get();
	}

	SharedMeshStore::SharedMeshStore() : m_bufferAllocator(std::make_unique<GLAllocator>(glGenBuffers, glDeleteBuffers))
	{
		m_dataStore.insert(
			{
				{
					MeshType::TRIANGLE,
					MeshData
					{
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.5f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 2, 1 }
					}
				},
				{
					MeshType::QUAD,
					MeshData
					{
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, 22.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 2, 1, 0, 3, 2 }
					}
				},
				{
					MeshType::CUBE,
					MeshData
					{
						{
							Vertex({ { -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),

							Vertex({ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),

							Vertex({ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),

							Vertex({ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),

							Vertex({ { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),

							Vertex({ { -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } })
						},
						{
							0, 2, 1, 0, 3, 2,
							4, 6, 5, 4, 7, 6,
							8, 10, 9, 8, 11, 10,
							12, 14, 13, 12, 15, 14,
							16, 18, 17, 16, 19, 18,
							20, 21, 22, 20, 22, 23
						}
					}
				},
				{
					MeshType::COLLIDER,
					MeshData
					{
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 1, 1, 2, 2, 3, 3, 0 }
					}
				}
			}
		);

		m_bufferAllocator->Reserve(MESH_COUNT << 1);
	}
}