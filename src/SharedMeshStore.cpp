#include <stdexcept>
#include <string>

#include "SharedMeshStore.h"

namespace diamond_engine {
	SharedMeshStore& SharedMeshStore::GetInstance() {
		static SharedMeshStore sharedMeshStore;

		return sharedMeshStore;
	}

	const std::shared_ptr<Mesh>& SharedMeshStore::FindMesh(Geometry geometry) {
		auto mesh = m_store.find(geometry);

		if (mesh == m_store.end()) {
			throw std::runtime_error(
				"Invalid Mesh geometry supplied: "
				+ std::to_string(static_cast<unsigned>(geometry)));
		}

		return m_store.at(geometry);
	}

	SharedMeshStore::SharedMeshStore() {
		m_store.insert({
			{
				Geometry::TRIANGLE,
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
				Geometry::QUAD,
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