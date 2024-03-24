#include "Collider2D.h"
#include "Collider2DBehaviour.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SharedMeshStore.h"
#include "SpriteAnimationPlayer.h"

namespace diamond_engine {
	Scene::Scene(const std::shared_ptr<SharedShaderStore>& sharedShaderStore, const std::shared_ptr<TextureLoader>& sharedTextureLoader, const std::shared_ptr<SpriteSheetLoader>& spriteSheetLoader) :
		m_sharedShaderStore(sharedShaderStore),
		m_sharedTextureLoader(sharedTextureLoader),
		m_sharedSpriteSheetLoader(spriteSheetLoader),
		m_vertexArrayAllocator(std::make_shared<GLAllocator>(glGenVertexArrays, glDeleteVertexArrays)),
		m_bufferAllocator(std::make_shared<GLAllocator>(glGenBuffers, glDeleteBuffers)),
		m_camera(std::make_shared<Camera>()),
		m_renderableObjectAllocator(std::make_shared<ObjectAllocator>()),
		m_spriteRenderSequence(std::make_unique<RenderSequence>()),
		m_collider2DRenderSequence(std::make_unique<RenderSequence>()),
		m_collisionResolver2D(std::make_unique<CollisionResolver2D>()) {
		m_camera->SetFocusTarget(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_camera->SetProjectionFrustum(45.0f, 1.333f, 0.3f, 1000.0f);

		const std::shared_ptr<ShaderProgram>& spriteShaderProgram = m_sharedShaderStore->FindProgram("sprite");

		if (!spriteShaderProgram) {
			throw std::runtime_error("Failed to locate \"sprite\" shader in shared shader store instance");
		}

		m_spriteRenderSequence->SetRenderDescriptor(
			{
				"projection",
				"cameraTranslation",
				"cameraRotation",
				"cameraScale",
				"cameraView",
				"model",
				"materialColor",
				"materialTextureOffset"
			}
		);
		m_spriteRenderSequence->SetShaderProgram(spriteShaderProgram);
		m_spriteRenderSequence->SetCamera(m_camera);
		m_spriteRenderSequence->SetObjectAllocator(m_renderableObjectAllocator);
		m_spriteRenderSequence->SetVertexArrayAllocator(m_vertexArrayAllocator);
		m_spriteRenderSequence->SetTextureLoader(m_sharedTextureLoader);

		m_collider2DRenderSequence->SetRenderDescriptor(
			{
				"projection",
				"cameraTranslation",
				"cameraRotation",
				"cameraScale",
				"cameraView",
				"model",
				"materialColor"
			}
		);

		const std::shared_ptr<ShaderProgram>& collider2DShaderProgram = m_sharedShaderStore->FindProgram("unlit_color");

		if (!spriteShaderProgram) {
			throw std::runtime_error("Failed to locate \"unlit_color\" shader in shared shader store instance");
		}

		m_collider2DRenderSequence->SetShaderProgram(collider2DShaderProgram);
		m_collider2DRenderSequence->SetCamera(m_camera);
		m_collider2DRenderSequence->SetObjectAllocator(m_renderableObjectAllocator);
		m_collider2DRenderSequence->SetVertexArrayAllocator(m_vertexArrayAllocator);
		m_collider2DRenderSequence->SetTextureLoader(m_sharedTextureLoader);
	}

	void Scene::SetMaxObjects(GLint maxObjects) {
		m_vertexArrayAllocator->Reserve(static_cast<GLsizei>(maxObjects));
		m_vertexArrayAllocator->Allocate(static_cast<GLsizei>(maxObjects));

		m_bufferAllocator->Reserve(static_cast<GLsizei>(maxObjects << 1));
		m_bufferAllocator->Allocate(static_cast<GLsizei>(maxObjects << 1));

		m_renderableObjectAllocator->Allocate(static_cast<size_t>(maxObjects));
		m_renderableObjectAllocator->Expand(static_cast<size_t>(maxObjects));
	}

	const std::shared_ptr<GLAllocator>& Scene::GetBufferAllocator() const {
		return m_bufferAllocator;
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject, const MaterialConfig& materialConfig, const glm::vec3& position, const glm::vec3& scale, GameObjectType type) {
		switch (type) {
		case GameObjectType::SPRITE: {
			Collider2D* collider2D = gameObject->GetComponent<Collider2D>("Collider2D");

			if (collider2D)
			{
				std::unique_ptr<GameObject> collider2DGameObject = std::make_unique<GameObject>();

				std::unique_ptr<MeshRenderer> collider2DMeshRenderer = std::make_unique<MeshRenderer>();
				collider2DMeshRenderer->SetVertexBufferObject(m_bufferAllocator->Get());
				collider2DMeshRenderer->SetElementBufferObject(m_bufferAllocator->Get());
				collider2DMeshRenderer->SetDrawMode(GL_DYNAMIC_DRAW);
				collider2DMeshRenderer->SetRenderMode(GL_LINES);
				collider2DMeshRenderer->SetMesh(SharedMeshStore::GetInstance().FindMesh(Shape::COLLIDER));
				collider2DGameObject->AcquireComponent(std::move(collider2DMeshRenderer));

				std::unique_ptr<Collider2DBehaviour> collider2DBehaviour = std::make_unique<Collider2DBehaviour>();
				collider2DBehaviour->setTrackingGameObject(gameObject.get());
				collider2DGameObject->AddBehaviour(std::move(collider2DBehaviour));

				MaterialConfig collider2DMaterialConfig;
				collider2DMaterialConfig.SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

				m_collider2DRenderSequence->AddGameObject(
					std::move(collider2DGameObject),
					collider2DMaterialConfig,
					position,
					scale);

				switch (collider2D->getColliderType()) {
				case ColliderType::CHARACTER:
					m_collisionResolver2D->addCharacterSprite(gameObject.get());
					break;
				case ColliderType::OBSTACLE:
					m_collisionResolver2D->addObstacleSprite(gameObject.get());
					break;
				default:
					break;
				}
			}

			SpriteAnimationPlayer* spriteAnimationPlayer = gameObject->GetComponent<SpriteAnimationPlayer>("AnimationPlayer");
			if (spriteAnimationPlayer)
			{
				spriteAnimationPlayer->setSharedSpriteSheetLoader(m_sharedSpriteSheetLoader);
			}

			m_spriteRenderSequence->AddGameObject(std::move(gameObject), materialConfig, position, scale);
			break;
		}
		default:
			break;
		}
	}

	void Scene::Update(GLfloat deltaTime) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_spriteRenderSequence->Update(deltaTime);

		m_collisionResolver2D->ResolveCollisions();

		m_collider2DRenderSequence->Update(deltaTime);

		m_spriteRenderSequence->Render();
		m_collider2DRenderSequence->Render();
	}

	void Scene::OnWindowResize(int width, int height) {
		m_camera->SetAspectRatio(static_cast<GLfloat>(width) / height);
	}
}