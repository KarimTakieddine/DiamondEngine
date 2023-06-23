#include <stdexcept>

#include "GameObjectBuilder.h"
#include "GameObjectConfig.h"
#include "MeshRendererBuilder.h"

namespace diamond_engine {
	void GameObjectBuilder::SetBufferAllocator(const std::shared_ptr<GLAllocator>& bufferAllocator) {
		m_bufferAllocator = bufferAllocator;
	}

	std::unique_ptr<GameObject> GameObjectBuilder::Build(const GameObjectConfig* gameObjectConfig) {
		std::unique_ptr<GameObject> result = std::make_unique<GameObject>();

		for (const auto& componentConfig : gameObjectConfig->GetComponentConfigs()) {
			switch (componentConfig->GetType()) {
			case ComponentType::MeshRenderer: {
				MeshRendererBuilder meshRendererBuilder;

				if (!m_bufferAllocator) {
					throw std::runtime_error("Null or no buffer allocator set on GameObjectBuilder instance");
				}

				meshRendererBuilder.SetBufferAllocator(m_bufferAllocator);

				result->AcquireComponent(meshRendererBuilder.Build(componentConfig.get()));
				break;
			}
			default:
				break;
			}
		}

		return result;
	}
}