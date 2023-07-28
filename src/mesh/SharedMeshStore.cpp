#include <stdexcept>
#include <string>

#include "mesh/SharedMeshStore.h"

namespace diamond_engine {
	/* static */ SharedMeshStore& SharedMeshStore::GetInstance() {
		static SharedMeshStore sharedMeshStore;

		return sharedMeshStore;
	}

	const std::shared_ptr<Mesh>& SharedMeshStore::FindMesh(Shape shape) {
		auto mesh = m_store.find(shape);

		if (mesh == m_store.end()) {
			throw std::runtime_error(
				"Invalid Mesh Shape supplied: "
				+ std::to_string(static_cast<unsigned>(shape)));
		}

		return m_store.at(shape);
	}

	SharedMeshStore::SharedMeshStore() {
		m_store.insert({
			{
				Shape::TRIANGLE,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } })
						},
						{ 0, 2, 1 }
					)
				)
			},
			{
				Shape::QUAD,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } })
						},
						{ 0, 2, 1, 0, 3, 2 }
					)
				)
			},
			{
				Shape::CUBE,
				std::shared_ptr<Mesh>(
					new Mesh(
						{
							Vertex({ { -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }),
							Vertex({ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }),
							Vertex({ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }),
							Vertex({ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } })
						},
						{ 0, 2, 1, 0, 3, 2, 1, 6, 5, 1, 2, 6, 4, 5, 6, 4, 6, 7, 0, 4, 7, 0, 7, 3, 3, 6, 2, 3, 7, 6, 4, 0, 1, 4, 1, 5 }
					)
				)
			}
		});
	}
}