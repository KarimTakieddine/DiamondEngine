#include <stdexcept>
#include <string>

#include "SharedMeshStore.h"

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
						{ 0, 1, 2 }
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
						{ 0, 1, 2, 0, 2, 3 }
					)
				)
			}
		});
	}
}