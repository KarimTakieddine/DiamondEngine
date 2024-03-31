#include <stdexcept>
#include <string>

#include "SharedMeshStore.h"

namespace diamond_engine {
	/* static */ SharedMeshStore& SharedMeshStore::GetInstance() {
		static SharedMeshStore sharedMeshStore;

		return sharedMeshStore;
	}

	const std::shared_ptr<Mesh>& SharedMeshStore::FindMesh(MeshType type) {
		auto mesh = m_store.find(type);

		if (mesh == m_store.end()) {
			throw std::runtime_error(
				"Invalid Mesh Shape supplied: "
				+ std::to_string(static_cast<unsigned>(type)));
		}

		return m_store.at(type);
	}

	SharedMeshStore::SharedMeshStore() {
		m_store.insert({
			{
				MeshType::TRIANGLE,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.5f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 2, 1 }
					)
				)
			},
			{
				MeshType::QUAD,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 2, 1, 0, 3, 2 }
					)
				)
			},
			{
				MeshType::CUBE,
				std::shared_ptr<Mesh>(
					new Mesh(
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
					)
				)
			},
			{
				MeshType::COLLIDER,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } })
						},
						{ 0, 1, 1, 2, 2, 3, 3, 0 }
					)
				)
			}
		});
	}
}