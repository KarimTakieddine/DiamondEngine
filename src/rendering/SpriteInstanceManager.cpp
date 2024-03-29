#include "MaterialRenderComponent.h"
#include "MeshRenderComponent.h"
#include "SharedMeshStore.h"
#include "SpriteInstanceConfig.h"
#include "SpriteInstanceManager.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	void SpriteInstanceManager::setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader)
	{
		m_sharedTextureLoader = sharedTextureLoader;
	}

	std::vector<std::unique_ptr<IRenderComponent>> SpriteInstanceManager::getComponents(const InstanceConfig* instanceConfig) const
	{
		const SpriteInstanceConfig* spriteInstanceConfig = dynamic_cast<const SpriteInstanceConfig*>(instanceConfig);

		if (!spriteInstanceConfig)
		{
			return { };
		}

		std::vector<std::unique_ptr<IRenderComponent>> result;

		std::unique_ptr<MeshRenderComponent> meshRenderComponent = std::make_unique<MeshRenderComponent>();
		// TODO
		//meshRenderComponent->setSharedMesh(SharedMeshStore::GetInstance().FindMesh(spriteInstanceConfig->getMeshRenderConfig().GetMeshType()));
		result.push_back(std::move(meshRenderComponent));

		std::unique_ptr<MaterialRenderComponent> materialRenderComponent = std::make_unique<MaterialRenderComponent>();
		// TODO
		materialRenderComponent->setTexture(m_sharedTextureLoader->GetTexture(spriteInstanceConfig->getMaterialConfig().GetTextureName()));
		result.push_back(std::move(materialRenderComponent));

		// TODO: Transform component as well

		return result;
	}
}