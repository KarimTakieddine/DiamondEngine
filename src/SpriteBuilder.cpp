#include "Material.h"
#include "MeshRenderer.h"
#include "SharedMeshStore.h"
#include "SpriteBuilder.h"
#include "Transform.h"

namespace diamond_engine {
	SpriteBuilder::SpriteBuilder() :
		m_vertexArrayAllocator(std::make_unique<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays)),
		m_bufferAllocator(std::make_unique<GLAllocator>(glGenBuffers, glDeleteBuffers)) { }

	void SpriteBuilder::SetCapacity(GLsizei capacity) {
		m_vertexArrayAllocator->Reserve(capacity);
		m_vertexArrayAllocator->Allocate(capacity);

		m_bufferAllocator->Reserve(capacity * 2);
		m_bufferAllocator->Allocate(capacity * 2);
	}

	void SpriteBuilder::AddComponents() {
		const int vertexArrayObject		= m_vertexArrayAllocator->Get();
		const int vertexBufferObject	= m_bufferAllocator->Get();
		const int elementBufferObject	= m_bufferAllocator->Get();

		std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(vertexArrayObject, vertexBufferObject, elementBufferObject);

		//meshRenderer->SetVertexShader(m_sharedShaderStore->FindShader(".\\shaders\\sprite\\vertex_transform.dmsh"));
		meshRenderer->SetMesh(SharedMeshStore::GetInstance().FindMesh(Shape::QUAD));
		meshRenderer->SetRenderMode(RenderMode::SOLID);
		meshRenderer->SetMeshType(MeshType::SPRITE);

		std::shared_ptr<Material> material = std::make_shared<Material>();
		//material->SetFragmentShader(m_sharedShaderStore->FindShader(".\\shaders\\sprite\\fragment_sprite.dmsh"));
		material->SetColor({ 1.0f, 0.0f, 1.0f });
		meshRenderer->SetMaterial(material);

		m_gameObject->AcquireComponent(std::move(meshRenderer));

		std::unique_ptr<Transform> transform = std::make_unique<Transform>();

		//m_gameObject->AcquireComponent(std::move(transform));
	}
}