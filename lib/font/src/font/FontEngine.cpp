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

		m_fonts.push_back(font);
		m_symbolToIndex.insert({ font.symbol, index });

		return { };
	}

	EngineStatus FontEngine::allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to allocate graphics memory. No rendering subsystem assigned", true };

		for (const auto& fontInstance : m_fontInstances)
		{
			renderingSubsystem->registerRenderObject("font_renderer", fontInstance->getRenderComponents());
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

	EngineStatus FontEngine::setTopLeft(const glm::vec2& topLeft)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			const GLfloat strideX = static_cast<GLfloat>(m_fontSize.width) / m_canvas.width;
			const GLfloat strideY = static_cast<GLfloat>(m_fontSize.height) / m_canvas.height;

			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setCTopLeft(topLeft);
			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setFontSize({ strideX, strideY });

			// TODO: Can re-use material for uvOffset

			fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setTexture(m_canvas);
		}

		return { };
	}

	void FontEngine::render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return;

		renderingSubsystem->render("font_renderer", m_fontInstances);
	}

	void FontEngine::printFont(GLubyte symbol, GLsizei row, GLsizei column)
	{
		auto indexIt = m_symbolToIndex.find(symbol);

		if (indexIt == m_symbolToIndex.cend())
			return;

		const auto& font = m_fonts[indexIt->second];

		const size_t rows		= static_cast<size_t>(static_cast<GLfloat>(m_canvas.height) / m_fontSize.height);
		const size_t columns	= static_cast<size_t>(static_cast<GLfloat>(m_canvas.width) / m_fontSize.width);

		if (row >= rows || column >= columns)
			return;

		const size_t index = (row * columns) + column;

		if (index >= m_fontInstances.size())
			return;

		const auto& fontInstance = m_fontInstances[index];

		FontRenderComponent* component	= fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent");
		const glm::vec2& fontSize		= component->getFontSize();

		component->setVXOffset({ column * fontSize.x, -row * fontSize.y });
		component->setUVOffset({ fontSize.x * font.column, fontSize.y * font.row });
		fontInstance->setActive(true);
	}

	void FontEngine::printString(const std::string& s, GLsizei startRow, GLsizei startColumn)
	{
		const GLsizei size = static_cast<GLsizei>(s.size());

		if (size == 0)
		{
			for (const auto& fontInstance : m_fontInstances)
			{
				fontInstance->setActive(false);
			}
		}

		// TODO: Smarter clearing logic. Shouldn't need to call print("")!

		// TODO: Don't need to re-calculate this...

		const GLsizei rows		= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.height) / m_fontSize.height);
		const GLsizei columns	= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.width) / m_fontSize.width);

		if (startRow >= rows || startColumn >= columns)
			return;

		const GLsizei columnRemainder	= size % rows;
		const GLsizei rowCount			= (size - columnRemainder) / rows;
		
		for (GLsizei row = startRow; row < rowCount; ++row)
		{
			for (GLsizei column = startColumn; column < columns; ++column)
			{
				const GLsizei index = ( row * columns ) + column;
				printFont(static_cast<GLubyte>(s[static_cast<size_t>(index)]), row, column);
			}
		}

		for (GLsizei column = startColumn; column < columnRemainder; ++column)
		{
			const GLsizei index = (rowCount * columns) + column;
			printFont(static_cast<GLubyte>(s[static_cast<size_t>(index)]), rowCount, column);
		}
	}

	EngineStatus FontEngine::configure(const Size& fontSize, const Texture& canvas)
	{
		m_symbolToIndex.clear();
		m_fontInstances.clear();
		m_fonts.clear();

		const size_t rows		= static_cast<size_t>(static_cast<GLfloat>(canvas.height) / fontSize.height);
		const size_t columns	= static_cast<size_t>(static_cast<GLfloat>(canvas.width) / fontSize.width);

		m_fontInstances.reserve(rows * columns);
		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t column = 0; column < columns; ++column)
			{
				std::unique_ptr<GameInstance> fontInstance = std::make_unique<GameInstance>();
				fontInstance->acquireRenderComponent(std::make_unique<FontRenderComponent>());
				fontInstance->acquireRenderComponent(std::make_unique<MaterialRenderComponent>());
				m_fontInstances.push_back(std::move(fontInstance));
			}
		}

		m_canvas	= canvas;
		m_fontSize	= fontSize;

		return { };
	}
}