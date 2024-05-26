#include "FontLibrary.h"
#include "FontRenderComponent.h"
#include "GameInstance.h"
#include "RenderingSubsystem.h"
#include "MaterialRenderComponent.h"
#include "TextWindow.h"

namespace diamond_engine
{
	EngineStatus TextWindow::setFontLibrary(const std::shared_ptr<FontLibrary>& fontLibrary)
	{
		if (!fontLibrary)
			return { "Attempt to link TextWindow to NULL font library", true };

		m_fontLibrary = fontLibrary;

		return { };
	}

	EngineStatus TextWindow::allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to allocate graphics memory. No rendering subsystem assigned", true };

		for (const auto& fontInstance : m_fontInstances)
		{
			renderingSubsystem->registerRenderObject("font_renderer", fontInstance->getRenderComponents());
		}

		return { };
	}

	EngineStatus TextWindow::releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
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

	EngineStatus TextWindow::setTopLeft(const glm::vec2& topLeft)
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

	EngineStatus TextWindow::setFontColor(const glm::vec3& fontColor)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setColor(fontColor);
		}

		return { };
	}

	EngineStatus TextWindow::setFontScale(const glm::vec2& fontScale)
	{
		for (const auto& fontInstance : m_fontInstances)
		{
			fontInstance->getRenderComponent<FontRenderComponent>("FontRenderComponent")->setFontScale(fontScale);
		}

		return { };
	}

	void TextWindow::render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return;

		renderingSubsystem->render("font_renderer", m_fontInstances);
	}

	void TextWindow::printFont(GLubyte symbol, GLsizei row, GLsizei column)
	{
		const auto font = m_fontLibrary->getFont(symbol);

		if (font.row == -1 || font.column == -1)
			return;

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

	void TextWindow::printString(const std::string& s)
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

		GLsizei& cursorRow		= m_cursorPos.height;
		GLsizei& cursorColumn	= m_cursorPos.width;

		// TODO: Don't need to re-calculate this...

		const GLsizei rows		= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.height) / m_fontSize.height);
		const GLsizei columns	= static_cast<GLsizei>(static_cast<GLfloat>(m_canvas.width) / m_fontSize.width);

		for (char c : s)
		{
			const GLubyte symbol = static_cast<GLubyte>(c);

			if (symbol == '\n')
			{
				if (cursorRow >= rows)
					cursorRow = 0;

				for (GLsizei i = cursorColumn; i < columns; ++i)
				{
					const GLsizei index = cursorRow * columns + i;

					m_fontInstances[index]->setActive(false);
				}

				++cursorRow;
				cursorColumn = 0;

				continue;
			}

			if (cursorColumn >= columns)
			{
				++cursorRow;

				cursorColumn = 0;
			}

			if (cursorRow >= rows)
			{
				cursorRow		= 0;
				cursorColumn	= 0;
			}

			printFont(symbol, cursorRow, cursorColumn);

			++cursorColumn;
		}
	}

	EngineStatus TextWindow::configure(const Size& fontSize, const Texture& canvas)
	{
		m_fontInstances.clear();

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