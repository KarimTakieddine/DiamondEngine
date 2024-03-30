#pragma once

#include "RenderInstanceManager.h"
#include "TextureLoader.h"

namespace diamond_engine
{
	class SpriteInstanceManager : public RenderInstanceManager
	{
	public:
		void setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader);

	protected:
		std::vector<std::unique_ptr<IRenderComponent>> getComponents(const GameInstanceConfig* instanceConfig) const final override;

	private:
		std::shared_ptr<TextureLoader> m_sharedTextureLoader{ nullptr };
	};
}
