#include "MaterialRenderComponent.h"
#include "MeshRenderComponent.h"
#include "SharedMeshStore.h"
#include "SpriteInstanceConfig.h"
#include "SpriteInstanceBuilder.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	void SpriteInstanceBuilder::setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader)
	{
		m_sharedTextureLoader = sharedTextureLoader;
	}

	void SpriteInstanceBuilder::setSharedBufferAllocator(const std::shared_ptr<GLAllocator>& sharedBufferAllocator)
	{
		m_sharedBufferAllocator = sharedBufferAllocator;
	}

	std::vector<std::unique_ptr<IRenderComponent>> SpriteInstanceBuilder::getRenderComponents(const GameInstanceConfig* instanceConfig) const
	{
		const SpriteInstanceConfig* spriteInstanceConfig = dynamic_cast<const SpriteInstanceConfig*>(instanceConfig);

		if (!spriteInstanceConfig)
		{
			return { };
		}

		std::vector<std::unique_ptr<IRenderComponent>> result;

		const MeshRenderConfig& meshRenderConfig					= spriteInstanceConfig->getMeshRenderConfig();
		std::unique_ptr<MeshRenderComponent> meshRenderComponent	= std::make_unique<MeshRenderComponent>();
		meshRenderComponent->setSharedBufferAllocator(m_sharedBufferAllocator);
		meshRenderComponent->setSharedMesh(SharedMeshStore::GetInstance().FindMesh(meshRenderConfig.GetMeshType()));
		meshRenderComponent->setDrawMode(meshRenderConfig.GetDrawMode());
		result.push_back(std::move(meshRenderComponent));

		const MaterialConfig& materialConfig								= spriteInstanceConfig->getMaterialConfig();
		std::unique_ptr<MaterialRenderComponent> materialRenderComponent	= std::make_unique<MaterialRenderComponent>();
		materialRenderComponent->setTexture(m_sharedTextureLoader->GetTexture(materialConfig.GetTextureName()));
		materialRenderComponent->setColor(materialConfig.GetColor());
		materialRenderComponent->setTextureOffset(materialConfig.GetTextureOffset());
		result.push_back(std::move(materialRenderComponent));

		// TODO: Transform component as well

		return result;
	}
}