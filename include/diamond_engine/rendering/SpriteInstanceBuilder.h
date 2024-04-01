#pragma once

#include "IGameInstanceBuilder.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	class SpriteInstanceBuilder : public IGameInstanceBuilder
	{
	public:
		~SpriteInstanceBuilder() final override = default;
		void setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader);
		void setSharedBufferAllocator(const std::shared_ptr<GLAllocator>& sharedBufferAllocator);
		std::vector<std::unique_ptr<IRenderComponent>> getRenderComponents(const GameInstanceConfig* instanceConfig) const final override;

	private:
		std::shared_ptr<TextureLoader> m_sharedTextureLoader{ nullptr };
		std::shared_ptr<GLAllocator> m_sharedBufferAllocator{ nullptr };
	};
}
