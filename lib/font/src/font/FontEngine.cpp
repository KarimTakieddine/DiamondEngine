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

		EngineStatus releaseStatus;
		for (auto fontInstanceIt = m_fontInstances.rbegin(); fontInstanceIt != m_fontInstances.rend(); ++fontInstanceIt)
		{
			releaseStatus = renderingSubsystem->unregisterRenderObject("font_renderer", (*fontInstanceIt)->getRenderComponents());

			if (!releaseStatus)
				return releaseStatus;
		}

		return renderingSubsystem->releaseVertexState();
	}

	EngineStatus FontEngine::setTopLeft(const glm::vec2& topLeft)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			const GLfloat strideX		= static_cast<GLfloat>(m_fontSize.width) / m_canvas.width;
			const GLfloat strideY		= static_cast<GLfloat>(m_fontSize.height) / m_canvas.height;
			const glm::vec2& fontScale	= fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->getFontScale();

			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setCTopLeft(topLeft + glm::vec2{-strideX * ( 1.0f - fontScale.x ), strideY * ( 1.0f - fontScale.y )});
			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setFontSize({ strideX, strideY });
			fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setTexture(m_canvas);
		}

		return { };
	}

	EngineStatus FontEngine::setFontColor(const glm::vec3& fontColor)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setColor(fontColor);
		}

		return { };
	}

	EngineStatus FontEngine::setFontScale(const glm::vec2& fontScale)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setFontScale(fontScale);
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
		fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setTextureOffset({ fontSize.x * font.column, fontSize.y * font.row });
		fontInstance->setActive(true);
	}

	void FontEngine::printString(const std::string& s)
	{
		const GLsizei size = static_cast<GLsizei>(s.size());

		if (size == 0)
		{
			for (const auto& fontInstance : m_fontInstances)
			{
				fontInstance->setActive(false);
			}

			m_cursorPos = { 0, 0 };
		}

		// TODO: Smarter clearing logic. Shouldn't need to call print("")!

		// TODO: Don't need to re-calculate this...

		const GLsizei rows		= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.height) / m_fontSize.height);
		const GLsizei columns	= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.width) / m_fontSize.width);

		GLsizei& startRow		= m_cursorPos.height;
		GLsizei& startColumn	= m_cursorPos.width;
		
		if (startColumn >= columns)
		{
			startColumn = 0;
			++startRow;
		}

		if (startRow >= rows)
		{
			startRow = 0;
		}

		const GLsizei columnRemainder	= size % rows;
		const GLsizei rowCount			= (size - columnRemainder) / rows;
		
		for (GLsizei row = 0; row < rowCount; ++row)
		{
			for (GLsizei column = 0; column < columns; ++column)
			{
				const GLsizei index = ( row * columns ) + column;
				printFont(static_cast<GLubyte>(s[static_cast<size_t>(index)]), startRow + row, startColumn + column);
			}
		}

		startRow += rowCount;

		for (GLsizei column = 0; column < columnRemainder; ++column)
		{
			const GLsizei index = (rowCount * columns) + column;
			printFont(static_cast<GLubyte>(s[static_cast<size_t>(index)]), startRow + rowCount, startColumn + column);
		}

		startColumn += columnRemainder;
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