#include "FontEngine.h"
#include "FontRenderComponent.h"
#include "GameInstance.h"
#include "RenderingSubsystem.h"
#include "MaterialRenderComponent.h"

namespace diamond_engine
{
	EngineStatus FontEngine::registerFont(const Font& font)
	{
		if (m_symbolToIndex.find(font.symbol) != m_symbolToIndex.end())
			return { "Failed to register font. Already registered: " + std::to_string(font.symbol), true };

		const GLsizei index = static_cast<GLsizei>(m_fonts.size());

		std::unique_ptr<GameInstance> fontInstance = std::make_unique<GameInstance>();
		fontInstance->acquireRenderComponent(std::make_unique<FontRenderComponent>());
		fontInstance->acquireRenderComponent(std::make_unique<MaterialRenderComponent>());
		m_fontInstances.push_back(std::move(fontInstance));

		m_fonts.push_back(font);
		m_symbolToIndex.insert({ font.symbol, index });

		return { };
	}

	EngineStatus FontEngine::allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem, const Texture& canvas)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to allocate graphics memory. No rendering subsystem assigned", true };

		for (const auto& pair : m_symbolToIndex)
		{
			const auto index = pair.second;

			const Font& font = m_fonts[index];
			const auto& fontInstance = m_fontInstances[index];

			renderingSubsystem->registerRenderObject("font_renderer", fontInstance->getRenderComponents());

			const GLfloat strideX = static_cast<GLfloat>(font.width) / canvas.width;
			const GLfloat strideY = static_cast<GLfloat>(font.height) / canvas.height;

			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setUVOffset({ strideX * index, strideY * index });
			fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setTexture(canvas);
		}

		return { };
	}

	EngineStatus FontEngine::releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to release graphics memory. No rendering subsystem assigned", true };

		// TODO: Releasing needs to be properly ordered and debugged for render components!

		EngineStatus releaseStatus;
		for (const auto& fontInstance : m_fontInstances)
		{
			releaseStatus = renderingSubsystem->unregisterRenderObject("font_renderer", fontInstance->getRenderComponents());

			if (!releaseStatus)
				return releaseStatus;
		}

		return renderingSubsystem->releaseVertexState();
	}

	void FontEngine::render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return;

		renderingSubsystem->render("font_renderer", m_fontInstances);
	}

	void FontEngine::printFont(const GLubyte symbol, GLsizei row, GLsizei column)
	{
		auto indexIt = m_symbolToIndex.find(symbol);

		if (indexIt == m_symbolToIndex.cend())
			return;

		m_fontInstances[indexIt->second]->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setVXOffset({ row * 0.0625f, row * 0.0625f });
	}
}